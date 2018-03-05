#ifndef ZHWK_DS_PROJ_SER_H
#define ZHWK_DS_PROJ_SER_H

#include "usermgr.h"
#include <stdio.h>

// return numbytes written
int save_users_to_file(const char* filename);

// not
int read_users_from_file(const char* filename);

#endif