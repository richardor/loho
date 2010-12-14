#include<stdio.h>
#include<stdlib.h>
#include"remote_control.h"
#include"light.h"
#include"lightoff_command.h"


int main(int argc , char *argv[])
{
	RemoteControl *remote_control = remote_control_create(5);
	Light *light = light_create("room light");
	Command *lightoff_command = lightoff_command_create(light);

	remote_control_set_command(remote_control, 0, NULL, lightoff_command);	
	
	remote_control_off_button_press(remote_control, 0);

	return 0;
}
