/*************************************
author:zhoumin
date:2010-7-26
usage:  将目录下的图片的大小信息写入一个txt文件
**************************************/ 
#include <stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include <string.h>
#include<stdlib.h>


FILE *m_file = NULL;
#define CREATE_FILE_NAME "picture_inf.txt"

static int file_create()
{	
	if(m_file == NULL)
	{
		m_file = fopen(CREATE_FILE_NAME, "wb");
		if(m_file == NULL)
		{
			printf("fopen the file(%s)failed!\n", CREATE_FILE_NAME);
			return -1;
		}
		return 0;
	}	
	return -1;
}
static int file_destroy()
{
	if(m_file)
	{
		fclose(m_file);
		m_file = NULL;
	}
	return 0;
}

static int file_write_file(char *string)
{	
	int ret = 0;
	if(string && m_file)
	{
		ret = fwrite(string, strlen(string), 1, m_file);
	}
	return 0;
}



static void usage(void)
{
	printf("usage: get_gif_width_height gif_dir\n");
}



static int is_gif_pic(char *file_name)
{
	if(file_name == NULL)
	{
		return -1;
	}

	if((strstr(file_name, ".gif\0") != NULL) || \
		(strstr(file_name, ".GIF\0") != NULL))
	{
		return 0;
	}

	return -1;
}

static int get_gif_width_height(char *file_name, int *width, int *height)
{
	if(!file_name || !width || !height)
	{
		printf("get_gif_width_height : null parameter!\n");
		return -1;
	}
	FILE *stream = NULL;
	unsigned char buf[16] = {0};
	
	stream = fopen(file_name,"r");
	if(stream == NULL)
	{
		printf("open the file (%s)failed\n", file_name);
		return -1;
	}
	fread(buf,16,1,stream);
	fclose(stream);
	
	if(buf[0] == 'G' && buf[1] == 'I' && buf[2] == 'F')
	{
		/*get width and height*/
		*width = (buf[7] << 8)| buf[6];
		*height  = (buf[9] << 8) | buf[8];
		return 0;

	}
	return -1;
}

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		usage();
		return -1;
	}

	DIR * dir = NULL;  
	struct dirent * ent = NULL;
	char *dir_name = NULL;
	int w = 0;
	int h = 0;
	char temp_buf[128] = {0};

	dir_name = argv[1];

	file_create();

	dir = opendir(dir_name);  
	if(NULL == dir)
	{
		printf("open the dir(%s)failed!\n", dir_name);
		file_destroy();
		return -1;
	}	
	while ((ent = readdir(dir))) 
	{  

		/*找到了一个文件*/
		if((strcmp(ent->d_name, ".") == 0) \
			|| (strcmp(ent->d_name, "..") == 0))
		{
			continue;
		}
		if(is_gif_pic(ent->d_name) == 0)
		{
			sprintf(temp_buf, "./%s/%s", dir_name, ent->d_name);
			get_gif_width_height(temp_buf, &w, &h);
			printf("w(%d), h(%d)\n", w, h);
			sprintf(temp_buf, "%s -- w[%d] h[%d]\n", ent->d_name, w, h);
			file_write_file(temp_buf);
		}
	}
	closedir(dir);
	dir_name = NULL;


	file_destroy();
	return 0;
}