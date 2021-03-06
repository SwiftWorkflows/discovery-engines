
AC_PREREQ([2.63])
AC_INIT([NFHEDM], [0.0], [wozniak@mcs.anl.gov])
AC_CONFIG_SRCDIR([src/FitOrientation.c])
AC_CONFIG_HEADERS([config.h])

# Set shared object suffix (.so on Linux, .dylib on Mac)
SO_SUFFIX=so
if [[ $(uname) = "Darwin" ]]
then
   SO_SUFFIX=dylib
fi

# Checks for programs.
AC_PROG_CC
AC_CHECK_PROG(ZSH, zsh, zsh, "no")
[[ ${ZSH} == "no" ]] && AC_MSG_ERROR([ZSH is required!])

LDFLAGS="${LDFLAGS} -lm"

COMPILER=GCC
AC_ARG_ENABLE(xlc,
    AS_HELP_STRING([--with-xlc], [use IBM XLC. You must still set CC.]),
    [
      COMPILER=XLC
    ])
AC_SUBST(COMPILER)

USE_DIP=0
AC_ARG_WITH(dip,
    AS_HELP_STRING([--with-dip], [location of DIP]),
    [
        DIP_HEADER=${withval}/include/dipio.h
        AC_MSG_CHECKING([${DIP_HEADER}])
        [[ -r ${DIP_HEADER} ]] && USE_DIP=1
        if [[ ${USE_DIP} == 0 ]]
        then
            AC_MSG_ERROR([Could not find DIP!])
        fi
        AC_MSG_RESULT([ok])
        CFLAGS="${CFLAGS} -I ${withval}/include"
        LDFLAGS="${LDFLAGS} -L ${withval}/lib -ldipio -ldip"
        RPATHS="${RPATHS} -Wl,-rpath -Wl,${withval}/lib"
    ])

USE_GSL=0
AC_ARG_WITH(gsl,
    AS_HELP_STRING([--with-gsl], [location of GSL]),
    [
        GSL_HEADER=${withval}/include/gsl/gsl_vector.h
        AC_MSG_CHECKING([${GSL_HEADER}])
        [[ -r ${GSL_HEADER} ]] && USE_GSL=1
        if [[ ${USE_GSL} == 0 ]]
        then
            AC_MSG_ERROR([Could not find GSL!])
        fi
        AC_MSG_RESULT([ok])
        CFLAGS="${CFLAGS} -I ${withval}/include"
        LDFLAGS="${LDFLAGS} -L ${withval}/lib -lgsl -lgslcblas"
        RPATHS="${RPATHS} -Wl,-rpath -Wl,${withval}/lib"
    ])

USE_NLOPT=0
AC_ARG_WITH(nlopt,
    AS_HELP_STRING([--with-nlopt], [location of NLopt]),
    [
        NLOPT_HEADER=${withval}/include/nlopt.h
        AC_MSG_CHECKING([${NLOPT_HEADER}])
        [[ -r ${NLOPT_HEADER} ]] && USE_NLOPT=1
        if [[ ${USE_NLOPT} == 0 ]]
        then
            AC_MSG_ERROR([Could not find NLopt!])
        fi
        AC_MSG_RESULT([ok])
        CFLAGS="${CFLAGS} -I ${withval}/include"
        LDFLAGS="${LDFLAGS} -L ${withval}/lib -lnlopt"
        RPATHS="${RPATHS} -Wl,-rpath -Wl,${withval}/lib"
    ])

# Checks for libraries.
AC_CHECK_LIB([dip], [dip_Initialise])
AC_CHECK_LIB([gsl], [gsl_vector])
AC_CHECK_LIB([nlopt], [nlopt_create])

# Checks for header files.
AC_CHECK_HEADERS([limits.h stdint.h stdlib.h string.h unistd.h])

m4_include([maint/find-tcl.m4])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# Checks for library functions...
# Fix autoconf malloc weirdness
# (http://wiki.buici.com/xwiki/bin/view/Programing+C+and+C%2B%2B/Autoconf+and+RPL_MALLOC):
export ac_cv_func_malloc_0_nonnull=yes
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([floor memset pow sqrt strchr strstr])

AC_SUBST(RPATHS)

ENABLE_OPENMP=0
AC_ARG_ENABLE(openmp,
              AS_HELP_STRING([--enable-openmp], [Use OpenMP?]),
              [], [enable_openmp="no"])
echo "Enable OpenMP: ${enable_openmp}"
if [[ "${enable_openmp}" == "yes" ]]
then
    ENABLE_OPENMP=1
else
    ENABLE_OPENMP=0
fi
AC_SUBST(ENABLE_OPENMP)
AC_DEFINE_UNQUOTED(HAVE_OPENMP,${ENABLE_OPENMP},[If we have OpenMP, 1, else 0.])

m4_include([m4/ax_lib_hdf5.m4])
AX_LIB_HDF5()
echo "HDF5 support: ${with_hdf5}"

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
