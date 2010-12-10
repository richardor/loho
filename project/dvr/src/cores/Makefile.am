SUBDIRS=. test
noinst_LIBRARIES=libcores.a
libcores_a_SOURCES=array_list.c clocks.c clocks_rtc.c
libcores_a_LIBADD=$(top_srcdir)/src/public/libpublic.a
INCLUDES=-I$(top_srcdir)/src/public -I$(top_srcdir)/driver/rtc
