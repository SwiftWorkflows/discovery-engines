#!/usr/bin/env python

import os, sys

def usage():
    print "usage: <sample>"

def abort(msg):
    print msg
    sys.exit(1)

def check(condition, msg):
    if not condition: abort(msg)
    
if len(sys.argv) != 2:
    usage()
    sys.exit(1)

sample = sys.argv[1]

check(os.path.isdir(sample), "Not a directory: " + sample)

for label in os.listdir(sample):
    labeldir = sample+"/"+label
    for temperature in os.listdir(labeldir):
        if os.path.isdir(labeldir+"/"+temperature):
            print('nxlink -s %s -l %s -d %s' %
                  (sample, label, temperature))

