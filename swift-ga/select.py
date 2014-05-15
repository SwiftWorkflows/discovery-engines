#!/usr/bin/env python

import sys

generation  = sys.argv[1]
winner_file = sys.argv[2]
count       = len(sys.argv)-3

# print ("count: "+str(count))
print ("winner_file: " + winner_file)

with open(winner_file, 'w') as f:
    f.write('d-'+str(generation)+'-2' + '\n')
