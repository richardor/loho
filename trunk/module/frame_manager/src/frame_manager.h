#ifndef __FRAME_MANAGER_H__
#define __FRAME_MANAGER_H__


typedef enum _week_{/*ÐÇÆÚ*/
    WK_SUNDAY,
    WK_MONDAY,
    WK_TUESDAY,
    WK_WEDNESDAY,
    WK_THURSDAY,
    WK_FRIDAY,
    WK_SATURDAY
}week_t;

typedef struct _datetime_{/*ÈÕÆÚ*/
    unsigned char year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
    week_t week;
}datetime_t;

struct _FrameManager;
typedef struct _FrameManager FrameManager;

FrameManager *frame_manager_create(unsigned char frame_num);
int frame_manager_add_frame(FrameManager *thiz, datetime_t date_time, bool i_frame);
int frame_manager_pop_frame(FrameManager *thiz, unsigned int num);
int frame_manager_get_current_date_and_time(FrameManager *thiz);

unsigned int frame_manager_get_write_pos(FrameManager *thiz);
unsigned int frame_manager_get_write_circuit_num(FrameManager *thiz);
unsigned int frame_manager_get_read_pos(FrameManager *thiz);
unsigned int frame_manager_get_read_circuit_num(FrameManager *thiz);

#endif
