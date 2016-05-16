#!/usr/bin/env python

# MK-LINKS
# Creates directories and links in APS FS that point back to DAQ

import os, stat, shutil, sys, time, glob, re


class mk_links:

    def __init__(self, src, dst):
        self.perms = stat.S_IRUSR | stat.S_IWUSR | stat.S_IXUSR | \
                     stat.S_IRGRP | stat.S_IWGRP | stat.S_IXGRP
        self.count = 0
        self.src = src
        self.dst = dst

    def usage(self):
        print "usage: <source directory (DAQ)> " + \
                     "<destination directory (APS)>" + \
                     "<subpath (sample/label/temperature)>"

    """ Create n directory up-links """
    def mk_ups(self, n):
        result = ""
        for i in range(0,n-1):
            result = result + "../"
        return result

    def go(self, parent, subpath, nxs_files, tiff_files):
        newdir = self.dst+subpath
        newdir = re.sub("data[12]/", "", newdir)
        print "newdir:",newdir
        if not os.path.exists(newdir):
            print "mkdir:", newdir
            os.makedirs(newdir)
            os.chmod(newdir, self.perms)
            self.copy_files(parent, subpath, nxs_files, tiff_files)
        return True

    def copy_files(self, parent, subpath, nxs_files, tiff_files):
        print "copy_files: subpath:", subpath

        for f in nxs_files:
            source_abs = f
            copy_name  = self.dst+parent+"/"+os.path.basename(f)
            copy_name = re.sub("data[12]/", "", copy_name)
            if not os.path.isfile(copy_name):
                print ("cp " + source_abs + "\n   " + copy_name)
                # time.sleep(1)
                shutil.copyfile(source_abs, copy_name)
                os.chmod(copy_name, self.perms)
            else:
                # print "exists:", copy_name
                pass

        for f in tiff_files:
            source_abs = self.src+subpath+"/"+f
            # print "source_abs:", source_abs

            copy_name = self.dst+subpath+"/"+f
            copy_name = re.sub("data[12]/", "", copy_name)
            copy_name = copy_name.replace(".tmp", "")

            if not os.path.islink(copy_name):
                # time.sleep(1) ; sys.stdout.flush()
                # print ("ln " + source_abs + "\n   " + copy_name)
                os.symlink(source_abs, copy_name)

if __name__ == "__main__":
    if len(sys.argv) != 4:
        mk_links().usage()
        sys.exit(1)
    src = sys.argv[1]
    dst = sys.argv[2]
    subpath = sys.argv[3]

    parent = os.path.dirname(subpath)
    parent = os.path.dirname(parent)
    parent = os.path.dirname(parent)

    pattern = src+parent+"/*.nxs"
    nxs_files = glob.glob(pattern)

    tiff_files = os.listdir(src+subpath)
    tiff_files.sort()
    mk_links(src,dst).go(parent, subpath, nxs_files, tiff_files)
