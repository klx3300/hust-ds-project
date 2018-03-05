#ifndef ZHWKRE_DS_PROJ_IOCTRL_H
#define ZHWKRE_DS_PROJ_IOCTRL_H

#include "zhwkre/bss.h"
#include <stdio.h>

qBinarySafeString readline(FILE* fp);
qBinarySafeString readall(FILE* fp);

#define STRAPPCH(bss,x) {\
char ch = (x);\
q__bss_append(&(bss),&ch,1);\
}

#endif