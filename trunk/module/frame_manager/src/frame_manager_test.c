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
	datetime_t date_time = {0};
	datetime_t cur_date_time = {0};
	int ret = -1;	

	frame_manager = frame_manager_create(8);

	date_time.year = 10;
	date_time.month = 1;
	ret = frame_manager_add_frame(frame_manager, date_time, true);
	date_time.month = 2;
	ret = frame_manager_add_frame(frame_manager, date_time, false);
	date_time.month = 3;
	ret = frame_manager_add_frame(frame_manager, date_time, false);
	date_time.month = 4;
	ret = frame_manager_add_frame(frame_manager, date_time, false);
	date_time.month = 5;
	ret = frame_manager_add_frame(frame_manager, date_time, false);
	date_time.month = 6;
	ret = frame_manager_add_frame(frame_manager, date_time, false);
	date_time.month = 7;
	ret = frame_manager_add_frame(frame_manager, date_time, true);
	date_time.month = 8;
	ret = frame_manager_add_frame(frame_manager, date_time, true);


	frame_manager_pop_frame(frame_manager, 1);
	ret = frame_manager_get_current_date_and_time(frame_manager, &cur_date_time);
	printf("ret(%d)cur_date_time year(%d) month(%d)\n", ret,cur_date_time.year, cur_date_time.month);

	date_time.month = 9;
	ret = frame_manager_add_frame(frame_manager, date_time, true);
	frame_manager_pop_frame(frame_manager, 1);
	ret = frame_manager_get_current_date_and_time(frame_manager, &cur_date_time);
	printf("ret(%d)cur_date_time year(%d) month(%d)\n", ret,cur_date_time.year, cur_date_time.month);

	date_time.month = 10;
	ret = frame_manager_add_frame(frame_manager, date_time, true);
	frame_manager_pop_frame(frame_manager, 1);
	ret = frame_manager_get_current_date_and_time(frame_manager, &cur_date_time);
	printf("ret(%d)cur_date_time year(%d) month(%d)\n", ret,cur_date_time.year, cur_date_time.month);

	date_time.month = 11;
	ret = frame_manager_add_frame(frame_manager, date_time, true);
	frame_manager_pop_frame(frame_manager, 1);
	ret = frame_manager_get_current_date_and_time(frame_manager, &cur_date_time);
	printf("ret(%d)cur_date_time year(%d) month(%d)\n", ret,cur_date_time.year, cur_date_time.month);

	date_time.month = 12;
	ret = frame_manager_add_frame(frame_manager, date_time, true);
	frame_manager_pop_frame(frame_manager, 1);
	ret = frame_manager_get_current_date_and_time(frame_manager, &cur_date_time);
	printf("ret(%d)cur_date_time year(%d) month(%d)\n", ret,cur_date_time.year, cur_date_time.month);


	date_time.month = 13;
	ret = frame_manager_add_frame(frame_manager, date_time, true);
	frame_manager_pop_frame(frame_manager, 1);
	ret = frame_manager_get_current_date_and_time(frame_manager, &cur_date_time);
	printf("ret(%d)cur_date_time year(%d) month(%d)\n", ret,cur_date_time.year, cur_date_time.month);

	date_time.month = 14;
	ret = frame_manager_add_frame(frame_manager, date_time, true);
	frame_manager_pop_frame(frame_manager, 1);
	ret = frame_manager_get_current_date_and_time(frame_manager, &cur_date_time);
	printf("ret(%d)cur_date_time year(%d) month(%d)\n", ret,cur_date_time.year, cur_date_time.month);

	date_time.month = 15;
	ret = frame_manager_add_frame(frame_manager, date_time, true);
	frame_manager_pop_frame(frame_manager, 1);
	ret = frame_manager_get_current_date_and_time(frame_manager, &cur_date_time);
	printf("ret(%d)cur_date_time year(%d) month(%d)\n", ret,cur_date_time.year, cur_date_time.month);

	date_time.month = 16;
	ret = frame_manager_add_frame(frame_manager, date_time, true);
	frame_manager_pop_frame(frame_manager, 1);
	ret = frame_manager_get_current_date_and_time(frame_manager, &cur_date_time);
	printf("ret(%d)cur_date_time year(%d) month(%d)\n", ret,cur_date_time.year, cur_date_time.month);

	date_time.month = 16;
	ret = frame_manager_add_frame(frame_manager, date_time, true);
	frame_manager_pop_frame(frame_manager, 1);
	ret = frame_manager_get_current_date_and_time(frame_manager, &cur_date_time);
	printf("ret(%d)cur_date_time year(%d) month(%d)\n", ret,cur_date_time.year, cur_date_time.month);


	date_time.month = 17;
	ret = frame_manager_add_frame(frame_manager, date_time, true);
	frame_manager_pop_frame(frame_manager, 1);
	ret = frame_manager_get_current_date_and_time(frame_manager, &cur_date_time);
	printf("ret(%d)cur_date_time year(%d) month(%d)\n", ret,cur_date_time.year, cur_date_time.month);

	date_time.month = 17;
	ret = frame_manager_add_frame(frame_manager, date_time, true);
	frame_manager_pop_frame(frame_manager, 1);
	ret = frame_manager_get_current_date_and_time(frame_manager, &cur_date_time);
	printf("ret(%d)cur_date_time year(%d) month(%d)\n", ret,cur_date_time.year, cur_date_time.month);


	return 0;
}

