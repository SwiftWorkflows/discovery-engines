
import sys

import pysro

if len(sys.argv) != 2:
    print "Requires exprmnt_file!"
    sys.exit(1)

exprmnt_file = sys.argv[1]

print "exprmnt_file: " + exprmnt_file

l, m, n = 1, 2, 3

pysro.compute_diff(exprmnt_file, l, m, n)

print("OK")
