
# PYSRO.PY

import _libpysro

# The problem
p = None

def problem_init():
    global p
    p = _libpysro.problem_make()
    _libpysro.problem_set(p,
                          -0.25, -0.25, -0.2260175, # a_o1v1, a_o2v2, a_o1v2
                          0, 0, 0, # l, m, n (dummies)
                          -5, -5, -5,     # hx0
                          5,  5,  5,     # hx1
                          10, 10, 10) # hxn

def compute_diff(exprmnt_file, l, m, n):
    global p
    if p == None:
        problem_init()
    _libpysro.problem_set_lmn(p, l,m,n)
    c = _libpysro.compute_diff(p, exprmnt_file)
    print("c: " + str(c))
    _libpysro.problem_free(p)
    return c
