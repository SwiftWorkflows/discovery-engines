
import os, stat, subprocess, sys, time

from mk_links_lib import mk_links

src = "/nfs/chess/daq/2015-3/a2/rosenkranz-311-1"
dst = "/nfs/chess/aps"

mt2 = "/home/wozniak/proj/d-e/mt2/bin/mt2"

perms = stat.S_IRUSR | stat.S_IWUSR | \
        stat.S_IRGRP | stat.S_IWGRP

def find_extension(src, extension):
    result = []
    for root, dirs, files in os.walk(src, followlinks=True):
        for f in files:
            if f.endswith(extension):
                result.append(root+"/"+f)
    return result

while True:
    os.chdir(src)

    # Create any required symlinks
    start = time.time()
    mk_links().go(src, dst)
    stop = time.time()
    print("mk_links time: %0.0f" % (stop-start))

    # Initialize our data structures for sorting
    not_stacked = dict()
    for i in range(1,101):
        not_stacked["f"+str(i)] = []

    # Look for scan.log files without a corresponding NXS file
    scan_logs = find_extension(src, ".log")
    for scan_log in scan_logs:
        fn_dir = os.path.dirname(scan_log)
        fn = os.path.basename(fn_dir) # f1,2,3
        if fn == "ceria": continue
        if "test" in scan_log: continue
        if "macros" in scan_log: continue
        tokens = fn_dir.split("/")
        n = len(tokens)
        tokens = tokens[6:n]
        subpath = "/".join(tokens)
        nxs_file = dst+"/"+subpath+".nxs" # May not yet exist
        if os.path.isfile(nxs_file):
            print "Found NXS:  " + nxs_file
        else:
            print "Found WORK: " + scan_log
            not_stacked[fn]+=[scan_log]

    # Print statistics
    msg = "status:"
    for i in range(1,4):
        fn = "f"+str(i)
        msg += " %s: %i" % (fn, len(not_stacked[fn]))
    print msg

    # Select a work item
    work = None
    for i in range(1,4):
        fn = "f"+str(i)
        L = not_stacked[fn]
        if len(L) != 0:
            work = L[0]
            break

    # If we found nothing, sleep and loop
    if work is None:
        print "SLEEP\n"
        time.sleep(60)
        continue

    print "selected:", work

    # Deconstruct/reconstruct file names
    tokens = work.split("/")
    n = len(tokens)
    fn = tokens[n-2]
    temperature = tokens[n-3]
    label = tokens[n-4]
    sample = tokens[n-5]
    root = tokens[0:n-5]
    dst_rk = dst + "/" + "rosenkranz-311-1"
    temperature_dir = dst_rk + "/" + "/".join([sample,label,temperature])
    fn_dir          = temperature_dir + "/" + fn
    
    # Create the TIFF list file
    tiffs = find_extension(fn_dir, ".tiff")
    tiffs.sort()
    nxs_file = temperature_dir+"/"+fn+".nxs"
    list_txt = "/tmp/wozniak-tagger-list.txt"
    with open(list_txt, "w") as fd:
        for tiff in tiffs:
            fd.write(tiff+"\n")

    # Issue the mt2 command
    params = " 2463 2527 3700 "
    stack_out = fn_dir+"/stack.out"
    command = mt2 + params + nxs_file + \
              " < " + list_txt +        \
              " > " + stack_out
    print "command:",command
    sys.stdout.flush()
    start = time.time()
    rc = subprocess.call(command, shell=True)
    stop = time.time()
    if not rc == 0:
        print "command failed!"
        sys.exit(1)
    os.chmod(nxs_file, perms)

    # Report statistics
    with open(stack_out, "a") as fd:
        fd.write("\nduration: %0.3f\n" % (stop-start))
        fd.write("timestamp: %0.3f\n" % time.time())

    # Stop looping if the user touched tagger.stop
    if os.path.isfile("/home/wozniak/tagger.stop"):
        print "Found tagger.stop!"
        sys.exit(0)
