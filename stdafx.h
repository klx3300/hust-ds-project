#ifndef ZHWK_DS_PROJ_STDAFX_H
#define ZHWK_DS_PROJ_STDAFX_H
int intcmp(void* a,unsigned int asz,void* b,unsigned int bsz);
int stringcmp(void* a,unsigned int asz,void* b,unsigned int bsz);

#define forset(desc,iter) for(qSetIterator iter=qSet_begin(desc);qSetIterator_isvalid(iter);iter=qSetIterator_next(iter))
#define formap(desc,iter) for(qBTreeIterator iter=qBTree_begin(desc);qBTreeIterator_isvalid(iter);iter=qBTreeIterator_next(iter))

#endif