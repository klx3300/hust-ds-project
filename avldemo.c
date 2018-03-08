#include "zhwkre/set.h"
#include "usermgr.h"
#include "ioctrl.h"
#include "serializer.h"
#include "stdafx.h"
#include "zhwkre/log.h"
#include "zhwkre/unidef.h"
#include "zhwkre/utils.h"
#include "zhwkre/error.h"
#include "zhwkre/bss.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

qBTreeDescriptor avltrees;

void dbgprintstr(char* str){
    printf("[");
    for(int i=0;str[i]!='\0';i++){
        printf("%u",(unsigned char)str[i]);
        if(str[i+1]!='\0') printf(",");
    }
    printf("]\n");
}

int getopercode(){
    qBinarySafeString inputs = readline(stdin);
    int tmp = 0;
    sscanf(inputs.str,"%d",&tmp);
    qbss_destructor(inputs);
    return tmp;
}

int getstring(char* dest,int limit){
    qBinarySafeString inputs = readline(stdin);
    int realcp = MAX(limit,inputs.size);
    strncpy(dest,inputs.str,realcp);
    dest[realcp] = '\0';
    qbss_destructor(inputs);
    return realcp;
}
int main() {
    avltrees = qBTree_constructor(intcmp);
    Usermgr_init();
    int op = 1;
    while (op) {
        FLAG_CMPDBG = 0;
        printf("\n\n");
        printf("             Set Operations               \n");
        printf("-------------------------------------------------\n");
        printf("    	  1. InitTree           \n");
        printf("    	  2. ListExistingTrees           \n");
        printf("    	  3. Dump          \n");
        printf("    	  4. Insert             \n");
        printf("    	  5. Delete                 \n");
        printf("    	  6. Traverse              \n");
        printf("    	  7. Union          \n");
        printf("    	  8. Difference          \n");
        printf("    	  9. Intersect    \n");
        printf("    	 10. DestroyTree \n");
        printf("    	 11. IsSubset \n");
        printf("    	 12. IsEqual \n");
        printf("    	  0. Exit\n");
        printf("-------------------------------------------------\n");
        printf("       Please choose your operation[0~10]:");
        op = getopercode();
        int i = 0;
        switch (op) {
            case 1: {
                printf("Please input tree ID(int)> ");
                int trid = getopercode();
                qBTreeIterator destiter;
                if(!qBTree__ptr_at(avltrees,&destiter,&trid,sizeof(trid))){
                    qLogFail("Tree already exist.");
                }
                // then create
                qSetDescriptor tmpdesc = qSet_constructor(intcmp);
                qBTree_insert(avltrees,trid,tmpdesc);
                break;
            }
            case 2: {
                formap(avltrees,trsiter){
                    printf("TREE %d size %d\n",*(int*)qBTreeIterator_deref(trsiter).key
                        ,((qSetDescriptor*)qBTreeIterator_deref(trsiter).value)->size);
                }
                break;
            }
            case 3:{
                printf("Please input tree ID(int)> ");
                int trid = getopercode();
                qBTreeIterator destiter;
                if(qBTree__ptr_at(avltrees,&destiter,&trid,sizeof(trid))){
                    qLogFail("Tree not exist.");
                    break;
                }
                qAVLTreeDescriptor *desc = qBTreeIterator_deref(destiter).value;
                printf("root > %d\n",*(int*)desc->root->data);
                forset(*desc,iter){
                    qAVLTreeNode *nd = iter.node;
                    printf("node > %d lchild %d rchild %d height %d\n",*(int*)nd->data,
                    nd->lchild == NULL?-1:*(int*)nd->lchild->data,nd->rchild==NULL?-1:*(int*)nd->rchild->data,
                    nd->height);
                }
                break;
            }
            case 4: {
                printf("Please input tree ID(int)> ");
                int trid = getopercode();
                qBTreeIterator destiter;
                if(qBTree__ptr_at(avltrees,&destiter,&trid,sizeof(trid))){
                    qLogFail("Tree not exist.");
                    break;
                }
                printf("Please input insert content(int)> ");
                int inscont = getopercode();
                if(qSet__insert(qBTreeIterator_deref(destiter).value,&inscont,sizeof(inscont))){
                    qLogWarnfmt("Element %d already exists.",inscont);
                }
                break;
            }
            case 5: {
                printf("Please input tree ID(int)> ");
                int trid = getopercode();
                qBTreeIterator destiter;
                if(qBTree__ptr_at(avltrees,&destiter,&trid,sizeof(trid))){
                    qLogFail("Tree not exist.");
                    break;
                }
                printf("Please input delete content(int)> ");
                int inscont = getopercode();
                qSetDescriptor *desc = qBTreeIterator_deref(destiter).value;
                qSetIterator it;
                if(qSet__ptr_at(*desc,&it,&inscont,sizeof(int))){
                    qLogWarnfmt("Element %d did not exist.",inscont);
                    break;
                }
                qSet__erase(desc,it);
                break;
            }
            case 6: {
                printf("Please input tree ID(int)> ");
                int trid = getopercode();
                qBTreeIterator destiter;
                if(qBTree__ptr_at(avltrees,&destiter,&trid,sizeof(trid))){
                    qLogFail("Tree not exist.");
                    break;
                }
                qSetDescriptor *desc = qBTreeIterator_deref(destiter).value;
                printf("{");
                forset(*desc,iter){
                    printf("%d%s",*(int*)iter.node->data,iter.node->next == NULL?"}":",");
                }
                break;
            }
            case 7: {
                printf("Please input tree ID(int)> ");
                int trida = getopercode();
                qBTreeIterator destitera;
                if(qBTree__ptr_at(avltrees,&destitera,&trida,sizeof(trida))){
                    qLogFail("Tree not exist.");
                    break;
                }
                printf("Please input tree ID(int)> ");
                int tridb = getopercode();
                qBTreeIterator destiterb;
                if(qBTree__ptr_at(avltrees,&destiterb,&tridb,sizeof(tridb))){
                    qLogFail("Tree not exist.");
                    break;
                }
                qSetDescriptor* desca = qBTreeIterator_deref(destitera).value;
                qSetDescriptor* descb = qBTreeIterator_deref(destiterb).value;
                qSetDescriptor un = qSet_union(*desca,*descb);
                printf("{");
                forset(un,iter){
                    printf("%d%s",*(int*)iter.node->data,iter.node->next == NULL?"}":",");
                }
                qSet_destructor(un);
                break;
            }
            case 8: {
                printf("Please input tree ID(int)> ");
                int trida = getopercode();
                qBTreeIterator destitera;
                if(qBTree__ptr_at(avltrees,&destitera,&trida,sizeof(trida))){
                    qLogFail("Tree not exist.");
                    break;
                }
                printf("Please input tree ID(int)> ");
                int tridb = getopercode();
                qBTreeIterator destiterb;
                if(qBTree__ptr_at(avltrees,&destiterb,&tridb,sizeof(tridb))){
                    qLogFail("Tree not exist.");
                    break;
                }
                qSetDescriptor* desca = qBTreeIterator_deref(destitera).value;
                qSetDescriptor* descb = qBTreeIterator_deref(destiterb).value;
                qSetDescriptor diff = qSet_difference(*desca,*descb);
                printf("{");
                forset(diff,iter){
                    printf("%d%s",*(int*)iter.node->data,iter.node->next == NULL?"}":",");
                }
                qSet_destructor(diff);
                break;
            }
            case 9: {
                printf("Please input tree ID(int)> ");
                int trida = getopercode();
                qBTreeIterator destitera;
                if(qBTree__ptr_at(avltrees,&destitera,&trida,sizeof(trida))){
                    qLogFail("Tree not exist.");
                    break;
                }
                printf("Please input tree ID(int)> ");
                int tridb = getopercode();
                qBTreeIterator destiterb;
                if(qBTree__ptr_at(avltrees,&destiterb,&tridb,sizeof(tridb))){
                    qLogFail("Tree not exist.");
                    break;
                }
                qSetDescriptor* desca = qBTreeIterator_deref(destitera).value;
                qSetDescriptor* descb = qBTreeIterator_deref(destiterb).value;
                qSetDescriptor inters = qSet_intersect(*desca,*descb);
                printf("{");
                forset(inters,iter){
                    printf("%d%s",*(int*)iter.node->data,iter.node->next == NULL?"}":",");
                }
                qSet_destructor(inters);
                break;
            }
            case 10: {
                printf("Please input tree ID(int)> ");
                int trid = getopercode();
                qBTreeIterator destiter;
                if(qBTree__ptr_at(avltrees,&destiter,&trid,sizeof(trid))){
                    qLogFail("Tree not exist.");
                    break;
                }
                qSetDescriptor *desc = qBTreeIterator_deref(destiter).value;
                qSet__destructor(desc);
                if(qBTree__erase(&avltrees,destiter)){
                    qLogFail("Unexpected erase failure.");
                }
                break;
            }
            case 11: {
                printf("Please input tree ID(int)> ");
                int trida = getopercode();
                qBTreeIterator destitera;
                if(qBTree__ptr_at(avltrees,&destitera,&trida,sizeof(trida))){
                    qLogFail("Tree not exist.");
                    break;
                }
                printf("Please input tree ID(int)> ");
                int tridb = getopercode();
                qBTreeIterator destiterb;
                if(qBTree__ptr_at(avltrees,&destiterb,&tridb,sizeof(tridb))){
                    qLogFail("Tree not exist.");
                    break;
                }
                qSetDescriptor* desca = qBTreeIterator_deref(destitera).value;
                qSetDescriptor* descb = qBTreeIterator_deref(destiterb).value;
                if(qSet_issubset(*desca,*descb)){
                    printf("%d is subset of %d!\n",trida,tridb);
                }else{
                    printf("%d is not subset of %d!\n",trida,tridb);
                }
                break;
            }
            case 12: {
                printf("Please input tree ID(int)> ");
                int trida = getopercode();
                qBTreeIterator destitera;
                if(qBTree__ptr_at(avltrees,&destitera,&trida,sizeof(trida))){
                    qLogFail("Tree not exist.");
                    break;
                }
                printf("Please input tree ID(int)> ");
                int tridb = getopercode();
                qBTreeIterator destiterb;
                if(qBTree__ptr_at(avltrees,&destiterb,&tridb,sizeof(tridb))){
                    qLogFail("Tree not exist.");
                    break;
                }
                qSetDescriptor* desca = qBTreeIterator_deref(destitera).value;
                qSetDescriptor* descb = qBTreeIterator_deref(destiterb).value;
                if(qSet_isequal(*desca,*descb)){
                    printf("%d is equal with %d!\n",trida,tridb);
                }else{
                    printf("%d is not equal with %d!\n",trida,tridb);
                }
                break;
            }
            case 0:{
                break;
            }
        }
    }

}