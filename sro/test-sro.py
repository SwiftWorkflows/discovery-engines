
import pysro

p = pysro.__sro_MOD_make_problem_c()
# print() # make_problem_c()

pysro.problem_set(p, 43.0, 43.0, 43.0, 43.0, 43.0, 43.0, 43.0, 43.0, 43.0, 43.0, 43.0, 43.0, 43, 43, 43)

pysro.problem_free(p)

# pysro.__sro_MOD_free_problem_c(p)
