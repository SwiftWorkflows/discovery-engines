#!/usr/bin/env python
#-----------------------------------------------------------------------------
# Copyright (c) 2013, NeXpy Development Team.
#
# Distributed under the terms of the Modified BSD License.
#
# The full license is in the file COPYING, distributed with this software.
#-----------------------------------------------------------------------------

import os, getopt, glob, re, sys, timeit
import numpy as np
from ConfigParser import ConfigParser
from nexpy.api.nexus import *
from nexpy.readers.tifffile import tifffile as TIFF

def get_prefixes(directory):
    prefixes = []
    for file in os.listdir(directory):
        f=file.split(os.path.extsep)[0]
        match = re.match('(.*?)([0-9]+)$', f)
        if match:
            prefixes.append(match.group(1).strip('-').strip('_'))
    return list(set(prefixes))

def get_files(directory, prefix, extension, reverse=False):
    if not extension.startswith('.'):
        extension = '.'+extension
    filenames = glob.glob(os.path.join(directory, prefix+'*'+extension))
    return sorted(filenames, key=natural_sort, reverse=reverse)

def get_metadata(metafile):
    parser = ConfigParser()
    if not os.path.exists(metafile):
        print "get_metadata: file does not exist: " + metafile
        exit(1)
    parser.read(metafile)
    return parser.getfloat('metadata','timeStamp'), \
           parser.get('metadata','dateString').replace(' : ','T').replace('.','-',2), \
           parser.getfloat('metadata', 'exposureTime'), \
           parser.getint('metadata', 'summedExposures')

def initialize_nexus_file(directory, prefix, filenames, omega, step):
    v0 = TIFF.imread(filenames[0])
    x = NXfield(range(v0.shape[1]), dtype=np.uint16, name='x_pixel')
    y = NXfield(range(v0.shape[0]), dtype=np.uint16, name='y_pixel')
    if len(filenames) > 1:
        z = omega+step*np.arange(len(filenames))
        if step < 0.0:
            z = z[::-1]
        z = NXfield(z, dtype=np.float32, name='rotation_angle', units='degree')
        v = NXfield(name='v',shape=(len(filenames),v0.shape[0],v0.shape[1]),
                    dtype=np.float32, maxshape=(5000,v0.shape[0],v0.shape[1]))
        data = NXdata(v, (z,y,x))
    else:
        v = NXfield(name='v',shape=(v0.shape[0],v0.shape[1]), dtype=np.float32)
        data = NXdata(v, (y,x))
    root = NXroot(NXentry(data,NXsample(),NXinstrument(NXdetector())))
    root.entry.instrument.detector.frame_start = \
        NXfield(shape=(len(filenames),), maxshape=(5000,), units='s', dtype=np.float64)
    filename = os.path.join(directory, prefix+'.nxs')
    try:
        root.save(filename, 'w')
    except IOError:
        print "Could not write to: " + filename
        print "PWD is: " + os.getcwd()
        exit(1)
    return root

def write_data(root, filenames, bkgd_root=None):
    metadata_file = filenames[0]+'.metadata'
    try:
        time, date, exposure, sum = get_metadata(metadata_file)
    except Exception as e:
        print e
        print "Caught error in: " + metadata_file
        print ""
        raise e

    root.entry.start_time = date
    root.entry.instrument.detector.frame_time = sum * exposure
    if bkgd_root:
        with bkgd_root.nxfile as f:
            f.copy('/entry',root.nxfile['/'],name='dark')
        frame_ratio = bkgd_root.entry.instrument.detector.frame_time /\
                      root.entry.instrument.detector.frame_time
        bkgd = bkgd_root.entry.data.v.nxdata.astype(np.float64) / frame_ratio
    else:
        bkgd = 0.0
    if len(root.entry.data.v.shape) == 2:
        root.entry.data.v[:,:] = TIFF.imread(filenames[0])
    else:
        chunk_size = root.nxfile['/entry/data/v'].chunks[0]
        for i in range(0, len(filenames), chunk_size):
            files = []
            try:
                for j in range(i,i+chunk_size):
                    print 'Processing', filenames[j]
                    files.append(filenames[j])
                    try:
                        time, date, exposure, sum = get_metadata(filenames[j]+'.metadata')
                        root.entry.instrument.detector.frame_start[j] = time
                    except Exception as error:
                        print filenames[j], error
            except IndexError:
                pass
            root.entry.data.v[i:i+len(files),:,:] = TIFF.TiffSequence(files).asarray() - bkgd

def write_metadata(root, directory, prefix):
    if 'dark' not in prefix:
        dirname=directory.split(os.path.sep)[-1]
        match = re.match('(.*?)_([0-9]+)k$', dirname)
        if match:
            try:
                sample = match.group(1)
                root.entry.sample.name = sample
                temperature = int(match.group(2))
                root.entry.sample.temperature = NXfield(temperature, units='K')
                root.entry.title = "%s %sK %s" % (sample, temperature, prefix)
            except:
                pass
    else:
        root.entry.sample.name = 'dark'
        root.entry.title = 'Dark Field'
    root.entry.filename = root.nxfilename

def natural_sort(key):
    import re
    return [int(t) if t.isdigit() else t for t in re.split(r'(\d+)', key)]

if __name__=="__main__":
    help = "merge_tiffs -d <directory> -e <extension> -p <prefix> -b <background> -o <omega> -s <step> -r"
    try:
        opts, args = getopt.getopt(sys.argv[1:],"hd:e:p:b:o:s:r",["directory=",
                                                "ext=","prefix=","background=",
                                                "omega=","step=","reversed"])
    except getopt.GetoptError:
        print help
        sys.exit(2)
    directory = './'
    extension = 'tif'
    prefix = None
    background = bkgd_data = None
    omega = 0.0
    step = 0.1
    reverse = False
    for opt, arg in opts:
        if opt == '-h':
            print help
            sys.exit()
        elif opt in ('-b', '--background'):
            background = arg
        elif opt in ('-p', '--prefix'):
            prefix = arg
        elif opt in ('-d', '--directory'):
            directory = arg
        elif opt in ('-e', '--extension'):
            extension = arg
        elif opt in ('-o', '--omega'):
            omega = np.float(arg)
        elif opt in ('-s', '--step'):
            step = np.float(arg)
        elif opt in ('-r', '--reversed'):
            reverse = True
    if prefix:
        prefixes = [prefix]
    else:
        prefixes = get_prefixes(directory)
    if background in prefixes:
        prefixes.insert(0,prefixes.pop(prefixes.index(background)))
    elif background:
        bkgd_root = nxload(os.path.join(directory,background+'.nxs'))
    for prefix in prefixes:
        tic=timeit.default_timer()
        data_files = get_files(directory, prefix, extension, reverse)
        root = initialize_nexus_file(directory, prefix, data_files, omega, step)
        if prefix == background:
            write_data(root, data_files)
            bkgd_root = root
        elif background:
            write_data(root, data_files, bkgd_root)
        else:
            write_data(root, data_files)
        write_metadata(root, directory, prefix)
        toc=timeit.default_timer()
        print toc-tic, 'seconds for', '%s.nxs' % prefix


