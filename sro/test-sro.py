
import sys

import pysro
import libpysro

if len(sys.argv) != 2:
    print "Requires exprmnt_file!"
    sys.exit(1)

exprmnt_file = sys.argv[1]

print "exprmnt_file: " + exprmnt_file

p = libpysro.problem_make()
libpysro.problem_set(p,
                  -0.25, -0.25, -0.2260175, # a_o1v1, a_o2v2, a_o1v2
                  10, 10, 10,     # l, m, n
                  -5, -5, -5,     # hx0
                   5,  5,  5,     # hx1
                  1000, 1000, 10) # hxn

c = libpysro.compute_diff(p, exprmnt_file)
print("c: " + str(c))

libpysro.problem_free(p)

print("OK")
