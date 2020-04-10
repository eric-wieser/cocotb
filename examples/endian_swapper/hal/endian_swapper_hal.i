// Swig template for this file

%module hal
%{
/* Includes the header in the wrapper code */
#include "endian_swapper_hal.h"
%}

/* Parse the header file to generate wrappers */
%include "endian_swapper_hal.h"
