#ifndef __LIGHT_H__
#define __LIGHT_H__


struct _Light;
typedef struct _Light Light;

Light *light_create(char *light_name);
int light_on(Light *thiz);
int light_off(Light *thiz);
void *light_destroy(Light *thiz);


#endif /*__LIGHT_H__*/
