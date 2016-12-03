
import pysro

p = pysro.problem_make()
# print() # problem_make_c()

pysro.problem_set(p,
                  -0.25, -0.25, -0.2260175, # a_o1v1, a_o2v2, a_o1v2
                  10, 10, 10,     # l, m, n
                  -5, -5, -5,     # hx0
                   5,  5,  5,     # hx1
                  1000, 1000, 10) # hxn

pysro.problem_free(p)

# pysro.__sro_MOD_problem_free_c(p)
