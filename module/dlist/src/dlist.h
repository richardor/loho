/******************************************************************************
 * Filename : dlist.h
 * Copyright: Copyright sz
 * Created : 2010-8-13 by zhoumin
 * Description -
 * 
 ******************************************************************************/
#include<stdio.h>
#include<stdlib.h>

#ifndef __DLIST_H__
#define __DLIST_H__


#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/



#define return_val_if_fail(p, val) if(!(p)) {printf("%s:%d "#p" failed.\n", __func__, __LINE__); return (val);}


typedef enum _RetDlist{
	RET_OK,
	RET_FAILED,
	RET_INVALIDPARAMETER,
}RetDlist;

typedef struct _DlistNode{
	struct _DlistNode *prev;
	struct _DlistNode *next;
	void *data;
	int length;
}DlistNode;

typedef struct _Dlist{
	DlistNode *list_node;	
}Dlist;

typedef RetDlist (*DlistDataPrintFunc)(void *ctx, void *data);


Dlist *dlist_create();

RetDlist dlist_append(Dlist *dlist, void *data);
RetDlist dlist_insert(Dlist *dlist, size_t index, void *data);
RetDlist dlist_print(Dlist *dlist, DlistDataPrintFunc print);

RetDlist dlist_destory(Dlist *dlist);




#ifdef __cplusplus
}
#endif/*__cplusplus*/


#endif

