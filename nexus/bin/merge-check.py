#!/usr/bin/env python

import glob
import io
import sys

def usage():
    print "merge-check.py <data> <listfile>"

if len(sys.argv) != 3:
    usage()
    exit(1)

data     = sys.argv[1]
listfile = sys.argv[2]

with open(listfile) as f:
    dirs = f.readlines()

prefixes = [ "fb1scan", "ff1scan", "ff2scan", "fb2scan", \
             "sfscan", "sbscan", "ubfscan", "ubbscan" ]

complete = 0
missing = 0

for d in dirs:
    d = d.strip()
    directory = data + "/" + d
    print directory
    m = 0
    c = 0
    for prefix in prefixes:
        pattern = directory + "/" + prefix + "*" + ".complete"
        found = glob.glob(pattern)
        if len(found) == 0:
            m = m + 1
        else:
            c = c + 1
    print "%i/%i" % (c, c+m)
    missing = missing + m
    complete = complete + c
print "%i/%i" % (complete, complete+missing)
