#include "../stdafx.h"
#include "../zhwkre/log.h"
#include "../zhwkre/unidef.h"
#include <string.h>

void dbgprintstr_(char* str){
    printf("[");
    for(int i=0;str[i]!='\0';i++){
        printf("%u",(unsigned char)str[i]);
        if(str[i+1]!='\0') printf(",");
    }
    printf("]\n");
}

int FLAG_CMPDBG = 0;


int intcmp(void* a,unsigned int asz,void* b,unsigned int bsz){
    return *(int*)a - *(int*)b;
}
int stringcmp(void* a,unsigned int asz,void* b,unsigned int bsz){
    char *achar=a,*bchar=b;
    int cmplen = MIN(strlen(achar),strlen(bchar));
    char cmpresult = 0;
    for(int i=0;i<cmplen;i++){
        if(achar[i]!=bchar[i]){
            cmpresult = achar[i]-bchar[i];
            if(FLAG_CMPDBG) qLogDebugfmt("[CH]Cmp %s %s result %d",achar,bchar,cmpresult);
            return cmpresult;
        }
    }
    int alen = strlen(achar),blen = strlen(bchar);
    if(alen != blen){
        cmpresult = alen - blen;
        if(FLAG_CMPDBG){
            qLogDebugfmt("[LN]Cmp %s %s result %d(%d-%d)",achar,bchar,cmpresult,alen,blen);
            dbgprintstr_(bchar);
        }
        return cmpresult;
    }
    if(FLAG_CMPDBG) qLogDebugfmt("[EQ]Cmp %s %s result 0",achar,bchar);
    return 0;
}