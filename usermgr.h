#ifndef ZHWK_HUST_DS_USERMGR_H
#define ZHWK_HUST_DS_USERMGR_H

#include "zhwkre/btree.h"
#include "zhwkre/bss.h"
#include "user.h"

// it is : <char username[USERNAMELEN],User user>
extern qBTreeDescriptor users;

void Usermgr_init();

void Usermgr_randomize(int seed);

int Usermgr_adduser(User user);
int Usermgr_removeuser(User user);
User* Usermgr_getuser(char* username);
qSetDescriptor Usermgr_bothwatch(User usera, User userb);
qSetDescriptor Usermgr_bothfriend(User usera, User userb);
qSetDescriptor Usermgr_2ndfriend(User user);
qBinarySafeString Usermgr_save();
int Usermgr_load(qBinarySafeString filecontent);

#endif