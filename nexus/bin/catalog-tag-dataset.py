#!/usr/bin/env python

import sys
import json
import re
import os
import ConfigParser
import io

from catalog_wrapper import *

def usage():
    print "usage: catalog-tag-dataset <directory>"

if len(sys.argv) != 2:
    usage()
    exit(1)

token_file = os.getenv('HOME','')+"/.ssh/gotoken.txt"
wrap = CatalogWrapper(token_file=token_file)

L = wrap.catalogClient.get_catalogs()
