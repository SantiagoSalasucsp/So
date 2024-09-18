#include <stdio.h>
#include "ucsp.h"

void fun(void) {
    printf("fun() called from a static library\n");
}

//make -f Makefile.static
//./main.static.exe

//make -f Makefile.dynamic
//export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
//./main.dynamic.exe

