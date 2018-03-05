#include "../stdafx.h"
#include "../zhwkre/log.h"
#include "../zhwkre/unidef.h"
#include <string.h>


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
            //qLogDebugfmt("Cmp %s %s result %d",achar,bchar,cmpresult);
            return cmpresult;
        }
    }
    int alen = strlen(achar),blen = strlen(bchar);
    if(alen != blen){
        cmpresult = alen - blen;
        //qLogDebugfmt("Cmp %s %s result %d",achar,bchar,cmpresult);
        return cmpresult;
    }
    //qLogDebugfmt("Cmp %s %s result 0",achar,bchar);
    return 0;
}