#include<stdio.h>
#include"file.h"


int main(int argc ,char *argv[])
{

	FileOperation *file_operation  = NULL;
	char buf[1024] = {0};
	char *file_name = "text.txt";
	
#if 0
	file_operation = file_fd_create("test.txt");
	file_write(file_operation, "hello world\n");
	file_write(file_operation, "aaaaaaaaaaaaaaaa\n");
	file_write(file_operation, "bbbbbbbbbbbbbbbbbbb\n");
	file_write(file_operation, "ccccccccccccccccccc\n");
	file_destroy(file_operation);
#endif
	
	file_operation = file_fd_create("a.txt");
	file_read(file_operation, buf, 1024);
	printf("buf(%s)\n", buf);
	file_destroy(file_operation);


	return 0;
}


