#ifndef __FILE_H__
#define __FILE_H__

#include<stdio.h>
#include<stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/


#define return_val_if_fail(p, val) if(!(p)) {printf("%s:%d "#p" failed.\n", __func__, __LINE__); return (val);}

typedef enum _RetFile{
	RET_OK,
	RET_FAILED,
	RET_INVALIDPARAMETER,
}RetFile;

typedef struct _FileOperation FileOperation;




typedef RetFile(*FileOperationWriteFunc)(FileOperation *thiz, void *ctx);
typedef RetFile(*FileOperationReadFunc)(FileOperation* thiz, void *ctx, size_t max_length);
typedef void (*FileOperationDestroyFunc)(FileOperation *thiz);

struct _FileOperation{
	FileOperationWriteFunc fl_write;
	FileOperationReadFunc fl_read;
	FileOperationDestroyFunc destroy;
	char priv[0];
};


static inline RetFile file_write(FileOperation *thiz, void *ctx)
{
	return_val_if_fail(thiz != NULL && thiz ->fl_write != NULL, RET_INVALIDPARAMETER);
	return 	thiz->fl_write(thiz, ctx);
}
static inline RetFile file_read(FileOperation* thiz, void *ctx, size_t max_length)
{
	return_val_if_fail(thiz != NULL && thiz->fl_read != NULL, RET_INVALIDPARAMETER);
	return 	thiz->fl_read(thiz, ctx, max_length);
}
static inline void file_destroy(FileOperation *thiz)
{
	if(thiz)
	{
		thiz->destroy(thiz);
	}
	return;
}




#ifdef __cplusplus
}
#endif/*__cplusplus*/



#endif
