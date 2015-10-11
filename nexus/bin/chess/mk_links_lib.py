
# MK-LINKS
# Creates directories and links in APS FS that point back to DAQ

import os, stat, shutil, sys, time

class mk_links:

    def __init__(self):
        self.perms = stat.S_IRUSR | stat.S_IWUSR | stat.S_IXUSR | \
                     stat.S_IRGRP | stat.S_IWGRP | stat.S_IXGRP
        self.count = 0
    
    def usage(self):
        print "usage: <source directory (DAQ)> " + \
                     "<destination directory (APS)>"

    """ Create n directory up-links""" 
    def mk_ups(self, n):
        result = ""
        for i in range(0,n-1):
            result = result + "../"
        return result

    def go(self, src, dst):
        for root,dirs,files in os.walk(src, followlinks=True):
            # print "root:", root
            tokens = root.split("/")
            #  print tokens
            n = len(tokens)
            tokens = tokens[7:n]
            # print tokens
            subpath = "/".join(tokens)
            newdir = dst+"/rosenkranz-311-1/"+subpath
            # print "newdir:",newdir
            if not os.path.exists(newdir):
                print "mkdir:", newdir
                os.mkdir(newdir)
                os.chmod(newdir, self.perms)
            self.copy_files(dst, files, tokens, n)

    def copy_files(self, dst, files, tokens, n):
        daq = "daq/current/a2/rosenkranz-311-1/"
        for f in files:
            # print "f:",f
            prefix = self.mk_ups(n-4)
            # print prefix
            # print tokens
            subpath = "/".join(tokens)
            # print "subpath:", subpath
            copy_name = dst+"/rosenkranz-311-1/"+subpath+"/"+f
            # print copy_name
            self.count += 1
            # if self.count == 10:
            #    sys.exit(0)
            # print ""
            if f.endswith(".nxs"):
                if not os.path.isfile(copy_name):
                    source_abs = "/nfs/chess/"+daq+subpath+"/"+f
                    print "f:",f
                    print "prefix:",prefix
                    print "tokens:",tokens
                    print "subpath:", subpath
                    print ("cp " + source_abs + "\n   " + copy_name)
                    # time.sleep(1)
                    shutil.copyfile(source_abs, copy_name)
                    os.chmod(copy_name, self.perms)
            else:
                if not os.path.islink(copy_name):
                    source_rel = prefix+daq+subpath+"/"+f
                    # time.sleep(1)
                    print ("ln " + source_rel + "\n   " + copy_name)
                    os.symlink(source_rel, copy_name)
                
if __name__ == "__main__":
    if len(sys.argv) != 3:
        usage()
        sys.exit(1)
    src = sys.argv[1]
    dst = sys.argv[2]
    ml = mk_links()
    ml.go(src,dst)
    print "count:", ml.count
    
