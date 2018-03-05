#ifndef ZHWK_DS_PROJ_USER_H
#define ZHWK_DS_PROJ_USER_H
#include "zhwkre/set.h"

#define USERNAMELEN 20

struct q__user{
    char username[USERNAMELEN];
    qSetDescriptor friends;
    qSetDescriptor watching;
    qSetDescriptor watcher;
};

typedef struct q__user User;

User User_constructor();

#endif