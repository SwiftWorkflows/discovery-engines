
import os, stat, subprocess, sys, time, datetime, glob

from mk_links_lib import mk_links

delay = int(sys.argv[1])

# src = "/nfs/chess/overflow/2016-2/a2/rosenkranz-311-2/data1/"
src = "/nfs/chess/overflow2/2016-2/a2/rosenkranz-311-2/data2/"
dst = "/nfs/chess/aps/rosenkranz-311-2/"

stack_started_file     = dst + "stack_started.txt"
stack_done_file        = dst + "stack_done.txt"
transform_done_file    = dst + "transform_done.txt"
scan_index_file = src + "scan_index.txt"
# scan_index_file = "/home/wozniak/aps/rosenkranz-311-2/scan_index_test.txt"

perms = stat.S_IRUSR | stat.S_IWUSR | \
        stat.S_IRGRP | stat.S_IWGRP

def now():
    return datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

print ""
print "TAGGER START: ", now()
print ""

def find_extension(src, extension):
    result = []
    for root, dirs, files in os.walk(src, followlinks=True):
        for f in files:
            if f.endswith(extension):
                result.append(root+"/"+f)
    return result

def slurp(filename):
    import re
    result = []
    with open(filename, "r") as f:
        for line in f:
            line = re.sub("#.*", "", line)
            line.strip()
            if len(line) > 0:
                result += [line.rstrip()]
    return result

def find_work():
    stack_started  = slurp(stack_started_file)
    index_news = []
    scan_index = slurp(scan_index_file);
    for line in scan_index:
        tokens = line.split(" ")
        if tokens[0] == "NEW:":
            assert len(tokens) == 3, "bad scan_index line: " + line
            subpath = tokens[1]
            count = int(tokens[2])
            index_news += [( subpath, count )]
    index_news.reverse()

    results = []

    for pair in index_news:
        subpath, count = pair
        # print "trying:", subpath
        if not subpath in stack_started:
            results += [pair]

        # for root,dirs,files in os.walk(src, followlinks=True):
        #     self.check(root,dirs,files)
    return results

while True:
    os.chdir(src)

    print ""
    print "ITERATION:" + now()
    print ""

    work_units = find_work()

    wus = []
    for entry in work_units:
        subpath, count = entry
        wus += [subpath]

    print "work units:", wus

    # Initialize our data structures for sorting
    not_stacked = dict()
    for i in range(1,101):
        not_stacked["f"+str(i)] = []

    # Process all work units
    for entry in work_units:
        subpath, count = entry
        fn_dir = os.path.dirname(subpath)
        fn = os.path.basename(fn_dir) # f1,2,3
        # if "test" in wu: continue
        if "macros" in subpath: continue
        # print "Found WORK: " + subpath
        not_stacked[fn] += [entry]

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
            subpath, count = L[0]
            break

    # If we found nothing, sleep and loop
    if work is None:
        sleep_time = 60
        print "WAITING: %i\n" % sleep_time; sys.stdout.flush()
        time.sleep(sleep_time)
        continue

    print "selected:", subpath

    stack_done     = slurp(stack_done_file)

    with open(stack_started_file, "a") as fd:
        fd.write(subpath + "\n")
    if not subpath in stack_done:
        print "processing:", subpath
        parent = os.path.dirname(subpath)
        parent = os.path.dirname(parent)
        parent = os.path.dirname(parent)
        # print parent
        pattern = src+parent+"/*.nxs"
        print pattern
        nxs_files = glob.glob(pattern)
        # print nxs_files
        tiff_files = os.listdir(src+subpath)
        tiff_files.sort()
        start = time.time()
        mk_links(src,dst).go(parent, subpath, nxs_files, tiff_files)
        stop = time.time()
        print("mk_links time: %0.0f" % (stop-start))

    # Deconstruct/reconstruct file names
    tokens = subpath.split("/")
    n = len(tokens)
    fn = tokens[n-2]
    temperature = tokens[n-3]
    label = tokens[n-4]
    sample = tokens[n-5]
    root = tokens[0:n-5]
    temperature_dir = dst + "/".join([sample,label,temperature])
    fn_dir          = temperature_dir + "/" + fn

    # Create the TIFF list file
    tiffs = find_extension(fn_dir, ".tiff")
    tiffs.sort()
    nxs_file = temperature_dir+"/"+fn+".nxs"
    list_txt = fn_dir+"/list.txt"
    with open(list_txt, "w") as fd:
        for tiff in tiffs:
            fd.write(tiff+"\n")

    # Create NXS and set perms before stack starts! No- confuses wf.sh
    # with open(nxs_file, "w") as f: pass
    # os.chmod(nxs_file, perms)

    # Issue the mt2 command
    params = " 2463 2527 " + str(count)
    stack_out = fn_dir+"/stack.out"
    command = "-f " + list_txt + " " + \
              params + " " + nxs_file

    wrapper_nxs = dst + "/".join([sample,label]) + "/" + sample+"_"+temperature+".nxs"

    # QSUB
    jobname = label+"-"+temperature
    command = "qsub-noblock " + \
              "-o " + temperature_dir+"/stack.out" + \
              " -- " + \
              temperature_dir + " " + jobname + " " + \
              "/home/wozniak/proj/d-e/nexus/bin/chess/wf.sh " + \
              wrapper_nxs + " " + subpath + " " + \
              command

    print "command:", command
    sys.stdout.flush()
    rc = subprocess.call(command, shell=True)
    if not rc == 0:
        print "command failed!"
        sys.exit(1)
    print "DELAYING: %i\n" % delay; sys.stdout.flush()
    time.sleep(delay)

    # Stop looping if the user touched tagger.stop
    if os.path.isfile("/home/wozniak/tagger.stop"):
        print "Found tagger.stop!"
        print "TAGGER STOP: ", now()
        sys.exit(0)
