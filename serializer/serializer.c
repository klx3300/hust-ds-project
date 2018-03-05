#include "../serializer.h"
#include "../stdafx.h"
#include "../zhwkre/unidef.h"
#include "../zhwkre/log.h"
#include "../ioctrl.h"
#include <stdio.h>
#include <string.h>

int save_users_to_file(const char* filename){
    qBinarySafeString bss = Usermgr_save();
    FILE* fp = fopen(filename,"w");
    if(fp == NULL){
        qLogFailfmt("Unable to open file %s.",filename);
        return 0;
    }
    fwrite(bss.str,bss.size,1,fp);
    fclose(fp);
    int bsssize = bss.size;
    qbss_destructor(bss);
    return bsssize;
}

int read_users_from_file(const char* filename){
    FILE* fp = fopen(filename,"r");
    if(fp == NULL){
        qLogFailfmt("Unable to open file %s.",filename);
        return 0;
    }
    qBinarySafeString fc = readall(fp);
    fclose(fp);
    int status = Usermgr_load(fc);
    qbss_destructor(fc);
    return status;
}