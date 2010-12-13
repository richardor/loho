#ifndef __ARRAY_LIST_H__
#define __ARRAY_LIST_H__
#include"public_typedef.h"

struct _ArrayList;
typedef struct _ArrayList ArrayList;



DECLS_BEGIN


ArrayList *array_list_create();

Ret array_list_add(ArrayList *thiz, void *data);
Ret array_list_remove(ArrayList *thiz, void *data, int data_size);
int array_list_get_id_by_data(ArrayList *thiz, void *data, int data_size);
void *array_list_get_data_by_id(ArrayList *thiz, int id);
int array_list_get_size(ArrayList *thiz);

void array_list_destroy(ArrayList *thiz);

DECLS_END


#endif /*__ARRAY_LIST_H__*/
