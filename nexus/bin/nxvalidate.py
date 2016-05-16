#!/usr/bin/env python

# from tifffile import *
import tifffile as tiff

import argparse, glob, os, random, subprocess
import numpy as np

from nexusformat.nexus import *
from nexusformat import nexus as nx

"""
usage: 
nxv = NXValidator()
nxv.setup(...) or nxv.parse_args()
nxv.validate()
"""
class NXValidator:

    def __init__(self):
        self.nxs_file = None
    
    def out(self, msg):
        if self.verbose:
            print msg

    def parse_args(self):
        parser = argparse.ArgumentParser(
            description="Perform validation for NeXus file " + \
                        "created from scan tiffs")
        parser.add_argument('-s', '--sample', help='sample name')
        parser.add_argument('-l', '--label', help='sample label')
        parser.add_argument('-t', '--temperature',
                            help='temperature of scan or other name')
        parser.add_argument('-f', '--f', help='f directory')
        parser.add_argument('-d', '--directory', default='',
                            help='scan directory containing tiffs')
        args = parser.parse_args()
        
        directory   = args.directory.rstrip('/')
        sample      = args.sample
        label       = args.label
        temperature = args.temperature
        f           = args.f
        points      = 10
        verbose     = True
        self.setup(directory, sample, label, temperature, f, \
                   points, verbose)
        
    def setup(self, directory, sample, label, temperature, f, \
              points, verbose):
        
        self.directory = directory
        self.sample = sample
        self.label = label
        self.temperature = temperature
        self.f = f
        self.points = points
        self.verbose = verbose
        
        if self.sample is None and self.label is None and \
           self.temperature is None:
            tokens = self.directory.split("/")
            n = len(tokens)
            self.sample      = tokens[n-3]
            self.label       = tokens[n-2]
            self.temperature = tokens[n-1]

        self.nxs_file = "/".join([self.sample,self.label]) + "/" + \
        self.sample + "_" + self.temperature + ".nxs"
        self.out("NeXus file:" + self.nxs_file)
        components = [self.sample, self.label, self.temperature]
        if not self.f == None:
            components += [self.f]
        # print components
        scan_dir = "/".join(components)
        pattern = scan_dir+"/*.tiff"
        self.out("tiff search:" + pattern)
        self.tiff_files = glob.glob(pattern)
        self.out("Processing sample '%s', label '%s', temperature '%s'" % \
                      (self.sample, self.label, self.temperature))

    def main(self):
        self.parse_args()
        try:
            self.validate()
        except NXValidationException as e:
            print str(e)
            exit(1)
        self.out("Validation succeeded.")

    def validate(self):

        seed = hash(self.nxs_file)
        random.seed(seed)
        tree = nxload(self.nxs_file)
        A = tree.f1.data.data

        X, Y, Z = A.shape

        for i in range(0,self.points):
           x = random.randint(0,X-1)
           y = random.randint(0,Y-1)
           z = random.randint(0,Z-1)
           print x, y, z, A[x,y,z]

           tiff_file = self.tiff_files[x]
           print tiff_file

           t = tiff.imread(tiff_file)
           # print t

           # print A[x,0,0], t[0,0]
           # print A[x,2,0], t[0,2]
           # print A[x,0,2], t[2,0]
           self.out("check: " + str((x,y,z, A[x,y,z], t[y,z])))

           # print A[x,y,z].__dict__

           # print A[x,y,z].__class__, t[y,z].__class__
           if not A[x,y,z]._value == t[y,z]:
               raise NXValidationException(nxs_file, x, y, z)
           # with TiffFile(tiff_files[z]) as tiff:
           #     # images = tiff.asarray()
           #     # image = images[0]
           #     # print image
           #     for page in tiff:
           #         print '\nPAGE\n'
           #         for tag in page.tags.values():
           #             t = tag.name, tag.value
           #             print t
           #         image = page.asarray()
           #         print image

class NXValidationException(Exception):
    def __init__(self, nxs_file, x, y, z):
        self.nxs_file = nxs_file
        self.coordinate = (x, y, z)
    def __str__(self):
        return repr("Validation failed on " + self.nxs_file + \
                    " at coordinate: " + str(self.coordinate))
            
if __name__=="__main__":
    nxv = NXValidator()
    nxv.main()
