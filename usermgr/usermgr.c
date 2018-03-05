#include "../usermgr.h"
#include "../stdafx.h"
#include "../ioctrl.h"
#include "../zhwkre/vector.h"
#include "../zhwkre/unidef.h"
#include "../zhwkre/log.h"
#include <stdlib.h>
#include <string.h>

qBTreeDescriptor users;

void Usermgr_init(){
    users = qBTree_constructor(stringcmp);
}

int Usermgr_adduser(User user){
    return qBTree__insert(&users,user.username,USERNAMELEN,&user,sizeof(User));
}
int Usermgr_removeuser(User user){
    qBTreeIterator targetit;
    int status = qBTree__ptr_at(users,&targetit,user.username,USERNAMELEN);
    if(status) return status;
    return qBTree__erase(&users,targetit);
}
User* Usermgr_getuser(char* username){
    qBTreeIterator targetit;
    int status = qBTree__ptr_at(users,&targetit,username,USERNAMELEN);
    if(status) return NULL;
    return qBTreeIterator_deref(targetit).value;
}
qSetDescriptor Usermgr_bothwatch(User usera, User userb){
    return qSet_intersect(usera.watching,userb.watching);
}
qSetDescriptor Usermgr_bothfriend(User usera, User userb){
    return qSet_intersect(usera.friends,userb.friends);
}
qSetDescriptor Usermgr_2ndfriend(User user){
    qSetDescriptor interdesc=qSet_constructor(stringcmp);
    for(qSetIterator it=qSet_begin(user.friends);qSetIterator_isvalid(it);it=qSetIterator_next(it)){
        qBTreeIterator informator;
        qBTree__ptr_at(users,&informator,qSetIterator_deref(it),USERNAMELEN);
        qSetDescriptor tmp = qSet_union(interdesc,(*(User*)qBTreeIterator_deref(informator).value).friends);
        qSet__destructor(&interdesc);
        interdesc = tmp;
    }
    qSetDescriptor tmp = qSet_difference(interdesc,user.friends);
    qSet__destructor(&interdesc);
    qSetIterator myselfit;
    if(!qSet__ptr_at(tmp,&myselfit,user.username,USERNAMELEN)){
        //!
        qSet__erase(&tmp,myselfit);
    }
    return tmp;
}
void append_set(qBinarySafeString* bss,qSetDescriptor set){
    q__bss_append(bss,VPTR(set.size),sizeof(set.size));
    for(qSetIterator it=qSet_begin(set);
    qSetIterator_isvalid(it);it=qSetIterator_next(it)){
        q__bss_append(bss,qSetIterator_deref(it),USERNAMELEN);
    }
}
// return readed bytes
int read_set(char* src,qSetDescriptor* set){
    char* position = src;
    unsigned int len = *(unsigned int*)position;
    position += sizeof(unsigned int);
    for(int i=0;i<len;i++){
        qSet__insert(set,position+i*USERNAMELEN,USERNAMELEN);
    }
    return position+len*USERNAMELEN-src;
}
// format of saving:
//                 friends      watching      watcher
// username -> [cnt][cnt*LEN][cnt][cnt*LEN][cnt][cnt*LEN] -> newline
qBinarySafeString Usermgr_save(){
    qBinarySafeString bss = qbss_constructor();
    for(qBTreeIterator it = qBTree_begin(users);
    qBTreeIterator_isvalid(it);it=qBTreeIterator_next(it)){
        User *usr = qBTreeIterator_deref(it).value;
        q__bss_append(&bss,usr->username,USERNAMELEN);
        append_set(&bss,usr->friends);
        append_set(&bss,usr->watching);
        append_set(&bss,usr->watcher);
    }
    return bss;
}
int Usermgr_load(qBinarySafeString content){
    char* position = content.str;
    while(position!=content.str+content.size){
        User usr=User_constructor();
        memcpy(usr.username,position,USERNAMELEN);
        position += USERNAMELEN;
        position=position + read_set(position,&(usr.friends));
        position=position + read_set(position,&(usr.watching));
        position=position + read_set(position,&(usr.watcher));
        int stat = Usermgr_adduser(usr);
        if(stat) return stat;
    }
    return 0;
}

#define vector_at(desc,pos) qVectorIterator_deref(qVectorIterator_move(qVector_begin(desc),pos))
#define myself(stra,strb) (stringcmp(stra,USERNAMELEN,strb,USERNAMELEN) == 0)
#define genstr(desc,lim) vector_at(desc,rand()%(lim))

void Usermgr_randomize(int seed){
    srand(seed);
    qVectorDescriptor allusername = qVector_constructor(USERNAMELEN);
    for(qBTreeIterator it = qBTree_begin(users);
    qBTreeIterator_isvalid(it);it=qBTreeIterator_next(it)){
        qVector__push_back(&allusername,qBTreeIterator_deref(it).key);
    }
    int eachcnts = MIN(users.size/3,30);
    int counter = 1;
    for(qBTreeIterator it = qBTree_begin(users);
    qBTreeIterator_isvalid(it);it=qBTreeIterator_next(it)){
        User* usr = qBTreeIterator_deref(it).value;
        qLogInfofmt("Generating dataset for %s (%d/%d)..",usr->username,counter,users.size);
        counter++;
        for(int i=0;i<eachcnts;i++){
            char* dst = genstr(allusername,users.size);
            if(!myself(dst,usr->username)){
                qSet__insert(&(usr->friends),dst,USERNAMELEN);
            }
            dst = genstr(allusername,users.size);
            if(!myself(dst,usr->username)){
                qSet__insert(&(usr->watching),dst,USERNAMELEN);
            }
            dst = genstr(allusername,users.size);
            if(!myself(dst,usr->username)){
                qSet__insert(&(usr->watcher),dst,USERNAMELEN);
            }
        }
    }
}
