
%module libpysro

%include "cfg.h"
%include "sro.h"

%{
#define SWIG_FILE_WITH_INIT
#include "sro.h"
%}
