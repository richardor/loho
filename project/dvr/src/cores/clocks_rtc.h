#ifndef __CLOCKS_RTC_H__
#define __CLOCKS_RTC_H__

#include "clocks.h"


DECLS_BEGIN

Clocks *clocks_rtc_create(char *rtc_dev_name);

DECLS_END

#endif /*__CLOCKS_RTC_H__*/
