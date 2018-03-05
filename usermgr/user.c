#include "../user.h"
#include "../stdafx.h"
#include <string.h>

User User_constructor(){
    User newusr;
    memset(&newusr,0,sizeof(User));
    newusr.friends = qAVLTree_constructor(stringcmp);
    newusr.watching = qAVLTree_constructor(stringcmp);
    newusr.watcher = qAVLTree_constructor(stringcmp);
    return newusr;
}