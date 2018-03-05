#include "../stdafx.h"
#include <string.h>


int intcmp(void* a,unsigned int asz,void* b,unsigned int bsz){
    return *(int*)a - *(int*)b;
}
int stringcmp(void* a,unsigned int asz,void* b,unsigned int bsz){
    char *achar=a,*bchar=b;
    int alen = strlen(a),blen = strlen(b);
    if(alen != blen) return alen-blen;
    for(int i=0;i<alen;i++){
        if(achar[i]!=bchar[i]) return achar[i]-bchar[i];
    }
    return 0;
}