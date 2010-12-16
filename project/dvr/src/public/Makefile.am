SUBDIRS=. test
noinst_LIBRARIES=libpublic.a
libpublic_a_SOURCES=public_function.c public_log.c

noinst_PROGRAMS=public_function_test public_log_test
public_function_test_LDADD=./libpublic.a
public_function_test_LDFLAGS=-lpthread

public_log_test_LDADD=./libpublic.a
