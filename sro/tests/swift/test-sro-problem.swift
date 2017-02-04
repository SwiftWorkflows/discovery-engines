import io;
import python;

printf(python_persist("""
import _libpysro
p = _libpysro.problem_make()
print "p is: " + str(p)
""",
"\"ok\""));
