#include"file_fd.h"
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<errno.h>



typedef struct _PrivInfo
{
        int  fd;
}PrivInfo;


static RetFile file_fd_write(FileOperation* thiz, void *ctx)
{
	printf("file_fd_write\n");
	return_val_if_fail(thiz != NULL && ctx != NULL, RET_INVALIDPARAMETER);
	
	char *string = NULL;
	int ret = 0;
	PrivInfo *priv = (PrivInfo *)thiz->priv;

	string = (char *)ctx;
	
	ret = write(priv->fd, ctx, strlen(ctx));
	
	return (ret == strlen(string)?RET_OK:RET_FAILED);
}

static RetFile file_fd_read(FileOperation* thiz, void *ctx, size_t max_length)
{
	printf("file_fd_read\n");
	return_val_if_fail(thiz != NULL && ctx != NULL, RET_INVALIDPARAMETER);
	return_val_if_fail(max_length > 0, RET_INVALIDPARAMETER);

	PrivInfo *priv = (PrivInfo *)thiz->priv;
	int ret = 0;
	
	ret = read(priv->fd, ctx, max_length);
	printf("ret(%d)ctx(%s)\n", ret, ctx);

	return (ret < 0)?RET_FAILED:RET_OK;
}

static void file_fd_destory(FileOperation* thiz)
{
	printf("file_fd_destory\n");
	if(thiz)
	{
		PrivInfo *priv = (PrivInfo *)thiz->priv;
		if(priv->fd >= 0)
		{
			close(priv->fd);
			priv->fd = -1;
		}
	}
	return;
}


FileOperation *file_fd_create(char *file_name)
{
	return_val_if_fail(file_name != NULL, NULL);
	FileOperation* thiz = (FileOperation*)malloc(sizeof(FileOperation) + sizeof(PrivInfo));
	PrivInfo *priv = (PrivInfo *)thiz->priv;

	if(thiz != NULL)
	{
		thiz->fl_write = file_fd_write;
		thiz->fl_read = file_fd_read;
		thiz->destroy = file_fd_destory;

		priv->fd = open(file_name, O_RDWR|O_CREAT);
		printf("file_fd_create priv->fd(%d)\n", priv->fd);
	}
	return priv->fd >= 0?thiz:NULL;
}

