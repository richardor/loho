#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/vfs.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/mount.h>
#include <sys/statfs.h>
#include"frame_manager.h"


int main(int argc , char *argv[])
{
	FrameManager *frame_manager = NULL;
	datetime_t date_time;
	int ret = -1;
	unsigned int write_pos = 0;
	int i = 0;

	frame_manager = frame_manager_create(8);


	for(i = 0; i < 7; i++)
	{
		printf("i(%d) aaaaaaaaaa wirte_pos = (%d) write circle(%d)\n",i, frame_manager_get_write_pos(frame_manager), frame_manager_get_write_circuit_num(frame_manager));
		ret = frame_manager_add(frame_manager, &date_time);
		printf("i(%d) bbbbbbbb wirte_pos = (%d)\n",i, frame_manager_get_write_pos(frame_manager));


	}
//	printf("aaaaaaaaaaaaa frame_manager read_pos(%d)\n", frame_manager_get_read_pos(frame_manager));
	ret = frame_manager_pop_frame(frame_manager, 2);
//	printf("bbbbbbbbbbbbbb frame_manager read_pos(%d)\n", frame_manager_get_read_pos(frame_manager));
		
	return 0;
}

