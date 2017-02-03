import io;
import python;

printf(python_persist("""
import libpysro
p = libpysro.problem_make()
print "p is: " + str(p)
""",
"\"ok\""));
