#!/usr/bin/env python

import sys
import h5py
import numpy as np

filename = sys.argv[1]
group    = sys.argv[2]

A = np.ndarray((10,10,10))
A[:,:,:] = 0
A[0,0,0] = 6
A[9,9,9] = 7
# print A[5,5,5]
f = h5py.File(filename, 'w')
g = f.create_group(group)
d = g.create_dataset('f', (10,10,10))
d[:,:,:] = A[:,:,:]
