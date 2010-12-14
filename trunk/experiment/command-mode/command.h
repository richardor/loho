#ifndef __COMMAND_H__
#define __COMMAND_H__

struct _Command;
typedef struct _Command Command;

typedef int (*CommandExecuteFunction)(Command *command);
typedef void (*CommandDestroyFunction)(Command *command);


struct _Command 
{
	CommandExecuteFunction execute;
	CommandDestroyFunction destroy;
	char *priv0;
};

static inline int command_execute(Command *thiz)
{
	return thiz->execute(thiz);
}

static inline void  command_destroy(Command *thiz)
{
	return thiz->destroy(thiz);
}

#endif	/*__COMMAND_H__*/
