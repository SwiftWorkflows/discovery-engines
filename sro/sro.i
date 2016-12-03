
%module pysro

%include "defns.h"
%include "sro.h"

%{
#define SWIG_FILE_WITH_INIT
#include "sro.h"
%}
