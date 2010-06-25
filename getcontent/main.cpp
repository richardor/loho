#include <stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>



#define CONTENT_FILE_NAME 	"content.txt"
#define HEADER_FILE_NAME		"header.txt"
#define COMMENT_FILE_NAME		"comment.txt"


#define CONTENT_KEYWORD 	"_CONTENT_=\""
#define HEADER_KEYWORD		"_HEADER_=\""
#define COMMENT_KEYWORD		"_COMMENT=_\""


char *write_to_filename[] = {
	"content.txt",
	"header.txt",
	"comment.txt"
};

char *keyword[] = {
	"_CONTENT_=\"",
	"_HEADER_=\"",
	"_COMMENT_=\"",
};

int main(int argc, char *argv[])
{
		if(argc < 2)
		{
			printf("usage: main xml_file_name\n");
			return 0;
		}

		printf("xml file name(%s)\n", argv[1]);
		
		FILE *stream = NULL;
	  char *text_buf = NULL;
	 	char *p_cur = NULL;
	 	char *p = NULL;
		char *p_temp = NULL;
	  char write_buf[120];
	  char write_buf_to_file[120];
	  FILE *fwrite_stream = NULL;
	  int conunt = 0;		
		
		
	  /*创建读文件的缓冲区*/
	  text_buf = new char[1024*1024];
	  if(text_buf == NULL)
	  {
	  	printf("malloc memory failed!\n");
			return -1;	
	  }
	  memset(text_buf, 0 , 1024*1024);


		/*将文本内容读入缓冲区*/	  
	  stream = fopen(argv[1],"r");

		fread(text_buf, 1024*1024,1,stream);  
		
	
//printf("text_buf(%s)\n", text_buf);
  for(int i = 0; i < 3; i++)
  {
  		
  	

	p = text_buf;

//	fwrite_stream = fopen(CONTENT_FILE_NAME, "wb");
	fwrite_stream = fopen(write_to_filename[i], "wb");
	
	if(fwrite_stream == NULL)
	{
		printf("fopen the file(%s) failed!\n",CONTENT_FILE_NAME);			
	}
	
	while(1)
	{
		printf("find the keyword!\n");
		/*查找KEYWORD*/
//		p = strstr(p, CONTENT_KEYWORD);
		p = strstr(p, keyword[i]);
		if( p == NULL){
			/*如果没有找到直接退出*/	
			break;
		}
		printf("find a keyword!\n");
		/*指针移到" 处*/
		p = strchr(p, '"');
		
		/*得到文本内容，也就是需要翻译的内容*/
		/*向前移动一个字符，过滤掉引号*/
		p++;
				
		p_temp = p;
		
		/*记录需要写入文本的字符个数*/
		conunt = 0;
		while(*p_temp != '"')
		{
			conunt++;
			p_temp++;
		}
		
		/*得到需要写入的内容*/
		strncpy(write_buf, p, conunt);
		
		write_buf[conunt] = '\0';
		
		printf("write_buf(%s), conunt(%d)", write_buf, conunt);
		
		sprintf(write_buf_to_file, "%s\r\n", write_buf);
		/*写入文件*/
		fwrite(write_buf_to_file, strlen(write_buf_to_file), 1, fwrite_stream);
		p = p_temp;
	}
		 sync();
		/*关闭打开的文件*/	   
	 fclose(fwrite_stream);
	 
  }
	 
	 fclose(stream);
 	
		return 0;
					

}
