#!/usr/bin/env python

import sys

cfg_input  = sys.argv[1]
hdf_output = sys.argv[2]
sh_output  = sys.argv[3]

execfile(cfg_input)

with open(sh_output, 'w') as f:
    f.write('export individuals='+ str(individuals) +'\n')
    f.write('export ngenerations='+str(ngenerations)+'\n')


