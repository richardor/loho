noinst_PROGRAMS=array_list_test clocks_rtc_test
array_list_test_SOURCES=array_list_test.c 

LIB_FLAG=$(top_srcdir)/src/cores/libcores.a

array_list_test_LDADD=$(LIB_FLAG)
INCLUDES=-I$(top_srcdir)/src/cores/ -I$(top_srcdir)/src/public

clocks_rtc_test_SOURCES=clocks_rtc_test.c
clocks_rtc_test_LDADD=$(LIB_FLAG)

