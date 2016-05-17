#!/usr/bin/env python

import tifffile as tiff

import argparse, glob, os, random, subprocess, sys
import numpy as np

from nexusformat.nexus import *
from nexusformat import nexus as nx

"""
Checks random points in given NeXus file against values in 
TIFF file directory.

command line usage: 
use nxvalidate -h

Python usage: 
nxv = NXValidator()
nxv.setup(...) or nxv.parse_args(...)
nxv.validate()
Returns nothing, raises NXValidationException on mismatch
"""
class NXValidator:

    def __init__(self):
        self.nxs_file = None

    """Verbose output"""
    def out(self, msg):
        if self.verbose:
            print msg
            sys.stdout.flush()

    """Internal method: construct and return an ArgumentParser"""
    def setup_parser(self, argv):
        parser = argparse.ArgumentParser(argv, 
                 description="Perform validation for NeXus file " + \
                             "created from scan tiffs",
                 epilog="Runs until given number of points have " + \
                        "been checked or number of nontrivials " + \
                        "have been checked.  " + \
                        "Here, nontrivial means > 0."
        )
        parser.add_argument('-s', '--sample', help='sample name')
        parser.add_argument('-l', '--label', help='sample label')
        parser.add_argument('-t', '--temperature',
                            help='temperature of scan or other name')
        parser.add_argument('-f', '--f', help='f directory')
        parser.add_argument('-d', '--directory', default='',
                            help='scan directory containing tiffs')
        parser.add_argument('-n', '--nontrivials', default='-1',
                            help='minimum number of nontrivial ' +
                            'points to check; if -1, check all ' +
                            'points.  Default -1')
        parser.add_argument('-p', '--points', default='-1',
                            help='number of points to check; ' +
                            'if -1, check X*Y*Z points.  Default -1')
        parser.add_argument('-v', '--verbose', action='store_true',
                            help='enable verbose output')
        return parser

    """Set up the Validator with a command line string"""
    def parse_args(self, argv=sys.argv):

        parser = self.setup_parser(argv)
        args = parser.parse_args()
        
        self.setup(args.directory.rstrip('/'),
                   args.sample, args.label,
                   args.temperature, args.f,
                   int(args.points), int(args.nontrivials),
                   args.verbose)

    """Set up the Validator with particular settings"""
    def setup(self, directory, sample, label, temperature, f, \
              points, nontrivials, verbose):
        
        self.directory   = directory
        self.sample      = sample
        self.label       = label
        self.temperature = temperature
        self.f           = f
        self.points      = points
        self.nontrivials = nontrivials
        self.verbose     = verbose
        
        if self.sample is None and self.label is None and \
           self.temperature is None:
            tokens = self.directory.split("/")
            n = len(tokens)
            self.sample      = tokens[n-3]
            self.label       = tokens[n-2]
            self.temperature = tokens[n-1]

        self.nxs_file = "/".join([self.sample,self.label]) + "/" + \
        self.sample + "_" + self.temperature + ".nxs"
        self.out("NeXus file: " + self.nxs_file)
        components = [self.sample, self.label, self.temperature]
        if not self.f == None:
            components += [self.f]
        scan_dir = "/".join(components)
        pattern = scan_dir+"/*.tiff"
        self.out("tiff search: " + pattern)
        self.tiff_files = glob.glob(pattern)
        self.out("Processing: " + \
                 "sample '%s', label '%s', temperature '%s'" % \
                 (self.sample, self.label, self.temperature))

    """Implement the command line interface"""
    def main(self):
        self.parse_args()
        try:
            self.validate()
        except NXValidationException as e:
            print str(e)
            sys.exit(1)
        self.out("Validation succeeded.")

    """Run the validation"""
    def validate(self):

        seed = hash(self.nxs_file)
        random.seed(seed)
        tree = nxload(self.nxs_file)
        A = tree[self.f].data.data
        X, Y, Z = A.shape

        if self.points == -1:
            self.points = X*Y*Z
        self.out("X,Y,Z: " + str((X, Y, Z)) + "\n")
            
        count     = 0 # Number of correct points
        nz_count  = 0 # Number of correct nontrivials
        x = y = z = 0 # Random sample
        while True:
            x = random.randint(0,X-1)
            y = random.randint(0,Y-1)
            z = random.randint(0,Z-1)
            self.out("check: " + str((x,y,z)))
            tiff_file = self.tiff_files[x]
            self.out("TIFF file: " + tiff_file)

            t = tiff.imread(tiff_file)

            nexus_value = A[x,y,z]._value
            tiff_value  = t[y,z]
            
            self.out("compare: NeXus: " + str(nexus_value) + \
                              " TIFF: " + str(tiff_value)  + "\n")
            if not nexus_value == tiff_value:
                raise NXValidationException(self.nxs_file, x, y, z)

            count += 1
            if t[y,z] > 0: nz_count += 1
            if count == self.points:
                break # Success!
            if self.nontrivials >= 0 and nz_count == self.nontrivials:
                break # Success!

        self.out("nontrivials matched: " + str(nz_count))

class NXValidationException(Exception):
    def __init__(self, nxs_file, x, y, z):
        self.nxs_file = nxs_file
        self.coordinate = (x, y, z)
    def __str__(self):
        return "Validation failed on " + self.nxs_file + \
               " at coordinate: " + str(self.coordinate)
            
if __name__=="__main__":
    nxv = NXValidator()
    nxv.main()
