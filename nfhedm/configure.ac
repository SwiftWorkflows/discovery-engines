
AC_PREREQ([2.68])
AC_INIT([NFHEDM], [0.0], [wozniak@mcs.anl.gov])
AC_CONFIG_SRCDIR([src/MakeHexGrid.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-ldip':
AC_CHECK_LIB([dip], [Dip_Initialise])
# FIXME: Replace `main' with a function in `-lnlopt':
AC_CHECK_LIB([nlopt], [nlopt_create])

# Checks for header files.
AC_CHECK_HEADERS([limits.h stdint.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([floor memset pow sqrt strchr strstr])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
