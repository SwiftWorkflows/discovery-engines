
# FIND-TCL.M4
# Finds Tcl settings for autotools

# In order from most to least preferred
TCL_VERSION_PREFS="8.6 8.5"

USE_TCL=0
USE_TCLSH_LOCAL=0

# Set USE_TCL
AC_ARG_WITH(tcl,
    AS_HELP_STRING([--with-tcl], [location of Tcl]),
    [
        USE_TCL=${withval}
    ],
    [
      AC_MSG_NOTICE([Tcl location not specified, trying to find tclsh on path])
      for TCLVER in ${TCL_VERSION_PREFS}
      do
        AC_MSG_CHECKING([for tclsh${TCLVER} on path])
        program=$( which tclsh${TCLVER} )
        if [[ ${?} == 0 ]]
        then
          AC_MSG_RESULT([yes: ${program}])
          USE_TCL=$( cd $(dirname ${program})/.. ; /bin/pwd )
          AC_MSG_RESULT([Found Tcl at ${USE_TCL}]);
          #AC_MSG_RESULT($(basename ${USE_TCLSH_LOCAL}))
          break
        else
          AC_MSG_RESULT([no])
        fi
      done

      if [[ ${USE_TCL} == 0 ]]
      then
        AC_MSG_ERROR([Could not find tclsh for any version: ${TCL_VERSION_PREFS}!])
      fi
    ]
)

# Sniff out default Tcl version and library path
TCL_VERSION=0

AC_ARG_WITH(tcl-version,
    AS_HELP_STRING([--with-tcl-version],
                   [Tcl version. Minimum is 8.5. If not specified,
                    use highest version we can find]),
    [
        TCL_VERSION=${withval}
        AC_MSG_NOTICE([specified Tcl version ${TCL_VERSION}])
        LIBTCL=libtcl${TCL_VERSION}.${SO_SUFFIX}
    ]
)

USE_TCL_LIB_DIR=0

AC_ARG_WITH(tcl-lib-dir,
    AC_HELP_STRING([--with-tcl-lib-dir],
                   [directory containing Tcl shared library]),
    [
        USE_TCL_LIB_DIR=${withval}
        AC_MSG_RESULT([using Tcl lib dir: ${USE_TCL_LIB_DIR}])
    ])

if [[ ${TCL_VERSION} != 0 ]]
then
  # We have locked in a version choice
  TCL_VERSION_PREFS=${TCL_VERSION}
fi

# Can always use static Tcl library
TCL_LIB_SUFFIX_PREFS="a"
if [[ "${ENABLE_SHARED}" != 0 ]]
then
  # Can use shared library in this case
  TCL_LIB_SUFFIX_PREFS="${SO_SUFFIX} ${TCL_LIB_SUFFIX_PREFS}"
fi

# Locate one or both variant of libtcl
USE_LIBTCL_A=
USE_LIBTCL_SO=

# Check in order from most to least preferred
for TCLVER in ${TCL_VERSION_PREFS}
do

  if [[ ${USE_TCL_LIB_DIR} == 0 ]]
  then
    LIB_DIR_OPTS="${USE_TCL}/lib ${USE_TCL}/lib64"

    # Debian distros may put in architecture-specific subdirectory
    if which dpkg-architecture > /dev/null
    then
      DPKG_ARCH=$(dpkg-architecture -qDEB_HOST_MULTIARCH)
      LIB_DIR_ARCH_OPTS=""
      for opt in $LIB_DIR_OPTS
      do
        LIB_DIR_ARCH_OPTS+=" $opt/${DPKG_ARCH}"
      done
      LIB_DIR_OPTS="$LIB_DIR_OPTS $LIB_DIR_ARCH_OPTS"
    fi
  else
    LIB_DIR_OPTS="${USE_TCL_LIB_DIR}"
  fi

  for TCL_LIB_SUFFIX in ${TCL_LIB_SUFFIX_PREFS}
  do
    for LIB_DIR in ${LIB_DIR_OPTS}
    do
      LIBTCL_PREFIX=libtcl${TCLVER}
      LIBTCL=${LIBTCL_PREFIX}.${TCL_LIB_SUFFIX}
      AC_MSG_CHECKING([for Tcl library at ${LIB_DIR}/${LIBTCL}])
      if [[ -r ${LIB_DIR}/${LIBTCL} ]]
      then
        AC_MSG_RESULT([exists!])
        TCL_VERSION=${TCLVER}
        USE_TCL_LIB_DIR=${LIB_DIR}

        # We found the library - work out which variants are present
        MAYBE_LIBTCL_SO="${LIB_DIR}/${LIBTCL_PREFIX}.${SO_SUFFIX}"
        MAYBE_LIBTCL_A="${LIB_DIR}/${LIBTCL_PREFIX}.a"
        if [[ -r "${MAYBE_LIBTCL_SO}" ]]
        then
          USE_LIBTCL_SO="${MAYBE_LIBTCL_SO}"
        fi
        if [[ -r "${MAYBE_LIBTCL_A}" ]]
        then
          USE_LIBTCL_A="${MAYBE_LIBTCL_A}"
        fi
        break 3
      else
        AC_MSG_RESULT([no])
      fi
    done
  done
done

if [[ -z "${USE_LIBTCL_A}" -a -z "${USE_LIBTCL_SO}" ]]
then
    AC_MSG_ERROR([could not find libtcl!])
fi

echo "using Tcl version: ${TCL_VERSION}"
echo "Tcl shared library: ${USE_LIBTCL_SO}"
echo "Tcl static library: ${USE_LIBTCL_A}"
AC_SUBST(TCL_VERSION)

# Set USE_TCL_CFG_DIR: location of tclConfig.sh
USE_TCL_CFG_DIR=0

# This is a location used e.g. by the APT tcl8.6-dev package.
# Put first to maximize chance of getting Tcl version correct
TCL_CFG_DIR_ALTS="${USE_TCL_LIB_DIR}/tcl${TCL_VERSION}"
# This location is for Tcl installation from source
TCL_CFG_DIR_ALTS+=" ${USE_TCL_LIB_DIR}"

for TCL_CFG_DIR_ALT in $TCL_CFG_DIR_ALTS
do
  AC_MSG_CHECKING([for tclConfig.sh at ${TCL_CFG_DIR_ALT}])
  if [[ -r "$TCL_CFG_DIR_ALT/tclConfig.sh" ]]
  then
    USE_TCL_CFG_DIR="${TCL_CFG_DIR_ALT}"
    AC_MSG_RESULT([found])
    break
  else
    AC_MSG_RESULT([no])
  fi
done

if [[ ${USE_TCL_CFG_DIR} == 0 ]]
then
    AC_MSG_ERROR([Could not find tclConfig.sh!])
fi
AC_MSG_RESULT([using tclConfig.sh in: ${USE_TCL_CFG_DIR}/])

# Make copy of old version since it can be clobbered by tclConfig.sh
TCL_OLD_VERSION="$TCL_VERSION"
source ${USE_TCL_CFG_DIR}/tclConfig.sh
if [[ ${?} != 0 ]]
then
    AC_MSG_ERROR([could not source: ${USR_TCL_CFG_DIR}/tclConfig.sh])
fi
if [[ "$TCL_VERSION" != "$TCL_OLD_VERSION" ]]
then
  AC_MSG_ERROR([${USE_TCL_CFG_DIR}/tclConfig.sh is not for appropriate \
      Tcl version.  Expected ${TCL_OLD_VERSION} but got ${TCL_VERSION}])
fi

# TCL_VERSION needed by find-tcl.zsh
export TCL_VERSION

# Find tclsh binary name (may be tclsh8.5)
if [[ ${USE_TCLSH_LOCAL} == 0 ]]
then
    USE_TCLSH=$( maint/find-tcl.zsh ${USE_TCL} )
    if [[ ${?} != 0 ]]
    then
        AC_MSG_ERROR([Could not find Tcl ${TCL_VERSION} binary in ${USE_TCL}!])
    fi
    USE_TCLSH_LOCAL=${USE_TCLSH}
else
    export NO_RUN=1
    USE_TCLSH=$( maint/find-tcl.zsh ${USE_TCL} )
    if [[ ${?} != 0 ]]
    then
        AC_MSG_ERROR([Could not find Tcl ${TCL_VERSION} binary at ${USE_TCL}!])
    fi
fi
AC_MSG_RESULT([using Turbine Tcl executable: ${USE_TCLSH}])

# Now, allow user to override Tcl include
AC_ARG_WITH(tcl-include,
            AS_HELP_STRING(
               [--with-tcl-include],
               [directory containing tcl.h]),
            [AC_CHECK_FILE(${withval}/tcl.h,[],[FAIL=1])
             [[ ${FAIL} == 1 ]] &&
               AC_MSG_ERROR(Could not find tcl.h in ${withval})
             AC_MSG_RESULT([using Tcl include: ${withval}/tcl.h])
             TCL_INCLUDE_SPEC=-I${withval}
            ],
            [])

AC_MSG_CHECKING([for Tcl system library directory with init.tcl])
USE_TCL_SYSLIB_DIR=
AC_ARG_WITH(tcl-syslib-dir,
      AS_HELP_STRING(
        [[--with-tcl-syslib-dir],
         [tcl system library directory.  Must contain init.tcl in \
          root or tcl${TCL_VERSION} subdirectory]]),
        [USE_TCL_SYSLIB_DIR="${withval}"])

if [[ -z "${USE_TCL_SYSLIB_DIR}" ]]
then
  # Put script in file as way to get tclsh to crash on invalid script
  tmp_script=$(mktemp)

  # Try to find out the library directory from tclsh
  echo 'puts [[ file dir [ info library ] ]]' > ${tmp_script}
  USE_TCL_SYSLIB_DIR=$( ${USE_TCLSH} $tmp_script )
  TCLSH_EXIT=$?
  rm $tmp_script
  if [[ $? != 0 ]]; then
    AC_MSG_ERROR([tcl-syslib-dir not provided and could not determine \
                  system library location by invoking ${USE_TCLSH}])
  fi
fi

if [[ ! -d "${USE_TCL_SYSLIB_DIR}" ]]
then
  AC_MSG_ERROR([Error finding tcl-syslib-dir: ${USE_TCL_SYSLIB_DIR} \
               not a directory])
fi

INIT_TCL_FOUND=0
# Try to locate init.tcl.  In some layouts it's in the root, in others
# it's in the tcl${TCL_VERSION} subdirectory
for syslib_dir in "${USE_TCL_SYSLIB_DIR}" \
                  "${USE_TCL_SYSLIB_DIR}/tcl${TCL_VERSION}"
do
  MAYBE_INIT_TCL="${syslib_dir}/init.tcl"
  if [[ -f ${MAYBE_INIT_TCL} ]]
  then
    INIT_TCL_FOUND=1
    AC_MSG_RESULT([yes])
    AC_MSG_RESULT([Found init.tcl: ${MAYBE_INIT_TCL}])
  fi
done

if (( ! INIT_TCL_FOUND ))
then
  AC_MSG_ERROR([could not locate init.tcl under ${USER_TCL_SYSLIB_DIR}:\
          appears not to be a legimitate Tcl system library directory])
fi

AC_MSG_RESULT([using Tcl system library directory: ${USE_TCL_SYSLIB_DIR}])

AC_SUBST(USE_TCL)
AC_SUBST(USE_TCL_LIB_DIR)
AC_SUBST(USE_TCL_SYSLIB_DIR)
AC_SUBST(USE_LIBTCL_A)
AC_SUBST(USE_LIBTCL_SO)
AC_SUBST(USE_TCLSH)
AC_SUBST(USE_TCLSH_LOCAL)
AC_SUBST(TCL_INCLUDE_SPEC)
AC_SUBST(TCL_LIB_SPEC)
# Tcl library dependencies for static build
TCL_LIB_SPEC_EXTRA="${TCL_LIBS}"
AC_SUBST(TCL_LIB_SPEC_EXTRA)
