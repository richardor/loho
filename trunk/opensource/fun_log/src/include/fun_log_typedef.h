#ifndef __FUN_LOG_TYPEDEF_H__
#define __FUN_LOG_TYPEDEF_H__




typedef enum _FunLogLevel_{
	LOG_ERROR,
	LOG_DEBUG,
	LOG_INVALID,
}FunLogLevel_t;


typedef enum _FunLogWriteMode_{
	LOG_PRINTF_MODE,			/*打印屏幕输出*/
	LOG_FILE_MODE,			/*保存文件输出*/
	LOG_SOCKET_MODE,			/*网络输出*/
	LOG_INVALID_MODE,		
}FunLogWriteMode_t;

#endif
