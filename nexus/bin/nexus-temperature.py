#!/usr/bin/env python

import sys

if len(sys.argv) < 2:
    print "Requires filename!"
    sys.exit(1)

filename = sys.argv[1]

from nexusformat.nexus.tree import NXFile
f = NXFile(filename, 'r')
T = f.readfile()
print T.entry.sample.temperature
