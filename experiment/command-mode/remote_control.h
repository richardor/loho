#ifndef __REMOTE_CONTROL_H__
#define __REMOTE_CONTROL_H__
#include"command.h"

struct _RemoteControl;
typedef struct _RemoteControl RemoteControl;


RemoteControl *remote_control_create();

void remote_control_set_command(RemoteControl *thiz, int slot, Command *on_command, Command *off_command);
void remote_control_on_button_press(RemoteControl *thiz, int slot);
void remote_control_off_button_press(RemoteControl *thiz, int slot);

void *remote_control_destroy(RemoteControl *thiz);















#endif /*__REMOTE_CONTROL_H__*/
