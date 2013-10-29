import os, getopt, glob, sys
import numpy as np
from libtiff import TIFF
from nexpy.api.nexus import *

def get_files(directory, prefix, extension):
    os.chdir(directory)
    if not extension.startswith('.'):
        extension = '.'+extension
    filenames = glob.glob(prefix+'*'+extension)
    return sorted(filenames,key=natural_sort)

def open_nexus_file(directory, prefix, filenames):
    im = TIFF.open(filenames[0])
    v0 = im.read_image()
    x = NXfield(range(v0.shape[1]), dtype=np.uint16, name='x')
    y = NXfield(range(v0.shape[0]), dtype=np.uint16, name='y')
    z = NXfield(range(1,len(filenames)+1), dtype=np.uint16, name='z')
    v = NXfield(name='v',shape=(len(filenames),v0.shape[0],v0.shape[1]),
                dtype=v0.dtype)
    data = NXdata(v, (z,y,x))
    nexus_file = os.path.join(directory, prefix+'.nxs')
    NXroot(NXentry(data)).save(nexus_file)
    return nexus_file

def write_data(nexus_file, filenames):
    root = nxload(nexus_file, 'rw')
    for i in range(len(filenames)):
        im = TIFF.open(filenames[i])
        v = im.read_image()
        root.entry.data.v.put(v, (i, 0, 0))
    root.save()

def natural_sort(key):
    import re
    return [int(t) if t.isdigit() else t for t in re.split(r'(\d+)', key)]    

if __name__=="__main__":
    try:
        opts, args = getopt.getopt(sys.argv[1:],"hd:e:",["directory=","ext="])
    except getopt.GetoptError:
        print 'merge_tiffs.py prefix -d <directory> -e <extension>'
        sys.exit(2)
    prefix = args[0]
    directory = ''
    extension = 'tif'
    for opt, arg in opts:
        if opt == '-h':
            print 'merge_tiffs.py prefix -d <directory> -e <extension>'
            sys.exit()
        elif opt in ("-d", "--directory"):
            directory = arg
        elif opt in ("-o", "--ofile"):
            extension = arg.strip('.')
    filenames = get_files(directory, prefix, extension)
    nexus_file = open_nexus_file(directory, prefix, filenames)
    write_data(nexus_file, filenames)
