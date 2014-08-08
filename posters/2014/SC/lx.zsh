#!/bin/zsh -fu

# Guide:
# Just use ./lx.zsh to compile to PDF.
# Use +f to force a recompile.
# Modify DOC to change the relevant tex file.
# Modify TMP & BIB to use different temporary storage.
# Use "./lx.zsh clean" to clean up

# set -x

DEFAULTDOC="nf-hedm"
COMPILER="pdflatex"

NEEDBIB="yes"
RECOMPILE="no"

TMP=.latex.out
BIB_OUT=.bibtex.out

VERBOSE=0

while getopts "bfv" OPTION
do
  case ${OPTION} in
    +f) RECOMPILE="yes" ;;
    -b) NEEDBIB="no"    ;;
    +b) NEEDBIB="yes"   ;;
    v)  ((VERBOSE++))   ;;
  esac
done
shift $(( OPTIND-1 ))
if (( ${#*} == 1 ))
then
  DOC=$1
fi

(( ${+DOC} )) || DOC=${DEFAULTDOC}

V=""
(( VERBOSE > 0 )) && V="-v"
(( VERBOSE > 1 )) && set -x

# Check if file $1 is uptodate wrt $2
# $1 is uptodate if it exists and is newer than $2
# If $2 does not exist, crash
uptodate()
{
  if (( ${#} < 2 ))
  then
    print "uptodate: Need at least 2 args!"
    return 1
  fi

  local OPTION
  local VERBOSE=0
  while getopts "v" OPTION
  do
    case ${OPTION}
      in
      v)
        (( VERBOSE++ )) ;;
    esac
  done
  shift $(( OPTIND-1 ))

  if (( VERBOSE > 1 ))
  then
    set -x
  fi

  local TARGET=$1
  shift
  local PREREQS
  PREREQS=( ${*} )

  local f
  for f in ${PREREQS}
  do
    if [[ ! -f ${f} ]]
    then
      (( VERBOSE )) && print "not found: ${f}"
      return 1
    fi
  done

  if [[ ! -f ${TARGET} ]]
  then
    print "does not exist: ${TARGET}"
    return 1
  fi

  local CODE
  for f in ${PREREQS}
  do
    [[ ${TARGET} -nt ${f} ]]
    CODE=${?}
    if (( ${CODE} == 0 ))
    then
      ((VERBOSE)) && print "${TARGET} : ${f} is uptodate"
    else
      ((VERBOSE)) && print "${TARGET} : ${f} is not uptodate"
      return ${CODE}
    fi
  done
  return ${CODE}
}

zclm()
# Select columns from input without awk
{
  local L
  local -Z c
  local A C i
  C=( ${*} )
  while read L
  do
    A=( $( print -- ${L} ) )
    N=${#C}
    for (( i=1 ; i<=N ; i++ ))
    do
      c=${C[i]}
      print -n "${A[c]}"
      (( i < N )) && print -n " "
    done
    printf "\n"
  done
  return 0
}

clean()
{
  local t
  setopt NULL_GLOB
  t=(  core* *.aux *.bbl *.blg *.dvi *.latex* *.log )
  t+=( *.toc *.lot *.lof .*.out )
  t+=( *.pdf *.ps )
  if [[ ${#t} > 0 ]]
  then
    rm -fv ${t}
  else
    print "Nothing to clean."
  fi
  return 0
}

scan()
{
  [[ $1 == "" ]] && return
  typeset -g -a $1
  local i=1
  while read T
  do
    eval "${1}[${i}]='${T}'"
    (( i++ ))
  done
}

shoot()
# print out an array loaded by scan()
{
  local i
  local N
  N=$( eval print '${#'$1'}' )
    # print N $N
  for (( i=1 ; i <= N ; i++ ))
  do
    eval print -- "$"${1}"["${i}"]"
  done
}

# Verbose operation
@()
{
  print
  print XXX ${*}
  print
  ${*}
}

check_bib_missing()
{
  awk '$0 ~ /Warn.*database entry/ { gsub(/\"/, "", $8); print "No entry for: " $8; }'
}

biblio()
{
  if [[ -f ${DOC}.bbl ]]
  then
    if ! uptodate ${V} ${DOC}.bbl ${DOC}.bib
    then
      rm ${V} ${DOC}.bbl
    fi
  fi
  if { bibtex ${DOC} >& ${BIB_OUT} }
   then
    printf "."
    ((VERBOSE)) && printf "\n"
    ${COMPILER} ${DOC} >& /dev/null
    printf "."
    ((VERBOSE)) && printf "\n"
    ${COMPILER} ${DOC} >& ${TMP}
    printf "."
    ((VERBOSE)) && printf "\n"
    check_bib_missing < ${BIB_OUT} | scan WARNS
    if (( ${#WARNS} > 0 ))
      then
      printf "\n"
      print "Bibtex:"
      shoot WARNS
    fi
  else
    printf "\n"
    cat ${BIB_OUT}
  fi
}

check_imgs()
{
  grep -h includegraphics *.tex | scan A
  IMGS=()
  for line in ${A}
  do
    PDF=( $( print ${${line/'{'/ }/'}'/ } ) )
    PDF=${PDF[2]}.pdf
    IMGS+=${PDF}
  done
  if (( ${#IMGS} > 0 ))
  then
    uptodate ${V} ${DOC}.pdf ${IMGS} || RECOMPILE="yes"
  fi
}

check_code()
{
  # Suppress boxes around code:
  export NO_BOX=1

  CODES=( $( grep -h "input code" ${DOC}.tex | zclm 2 ) )
  for C in ${CODES}
  do
    # Handle input file that may or may not have suffix .tex
    SRC=${C%.tex}
    C=${C}.tex
    # SRC file does not need to exist:
    [[ -f ${SRC} ]] || continue
    if ! uptodate ${V} ${C} ${SRC} code/script2tex_swift.pl
    then
      print "generating: ${SRC}.tex"
      code/script2tex_swift.pl ${SRC} > ${C}
    fi
  done
  if (( ${#CODES} > 0 ))
  then
    uptodate ${V} ${DOC}.pdf ${CODES} || RECOMPILE="yes"
  fi
}

[[ ${DOC} == "clean" ]] && clean && exit

check_code
if [[ ${RECOMPILE} == "no" ]]
then
  [[ -f ${DOC}.pdf ]] || RECOMPILE="yes"
fi
if [[ ${RECOMPILE} == "no" ]]
then
  [[ -f error ]] && RECOMPILE="yes"
fi
if [[ ${RECOMPILE} == "no" ]]
then
  uptodate ${V} ${DOC}.pdf ${DOC}.tex ${DOC}.bib lx.zsh || \
    RECOMPILE="yes"
fi
if [[ ${RECOMPILE} == "no" ]]
then
  check_imgs
fi

if [[ ${RECOMPILE} == "yes" ]]
 then
  ((VERBOSE)) && print "LaTeX ..."
  if { ${COMPILER} --interaction nonstopmode ${DOC} >& ${TMP} }
   then
    printf "OK"
    ((VERBOSE)) && printf "\n"
    rm -f error
    [[ ${NEEDBIB} == "yes" ]] && biblio
  else
    printf "Error! \n"
    egrep '^l.|^!|argument' ${TMP}
    touch error
  fi
fi

printf "\n"
grep "LaTeX Warning:" ${TMP}

return 0
