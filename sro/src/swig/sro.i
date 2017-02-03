
%module libpysro

%include "src/cfg.h"
%include "src/c/sro.h"

%{
#define SWIG_FILE_WITH_INIT
#include "src/c/sro.h"
%}
