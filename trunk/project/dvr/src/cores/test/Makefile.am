noinst_PROGRAMS=array_list_test clocks_rtc_test print_observer_test
array_list_test_SOURCES=array_list_test.c 

LIB_FLAG=$(top_srcdir)/src/cores/libcores.a\
		$(top_srcdir)/src/public/libpublic.a

array_list_test_LDADD=$(LIB_FLAG)
INCLUDES=-I$(top_srcdir)/src/cores/ -I$(top_srcdir)/src/public

clocks_rtc_test_SOURCES=clocks_rtc_test.c
clocks_rtc_test_LDADD=$(LIB_FLAG)
clocks_rtc_test_LDFLAGS=-lpthread

print_observer_test_SOURCES=print_observer_test.c
print_observer_test_LDADD=$(LIB_FLAG)



