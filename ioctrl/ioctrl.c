#include "../ioctrl.h"

qBinarySafeString readline(FILE* fp){
    qBinarySafeString result = qbss_constructor();
    for(int ch='\0';ch!=EOF && ch!='\n';ch=fgetc(fp)){
        char realch = (char)ch;
        q__bss_append(&result,&realch,1);
    }
    return result;
}
qBinarySafeString readall(FILE* fp){
    qBinarySafeString result = qbss_constructor();
    for(int ch='\0';ch!=EOF;ch=fgetc(fp)){
        char realch = (char)ch;
        q__bss_append(&result,&realch,1);
    }
    return result;
}