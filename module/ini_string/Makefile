CROSS_COMPILER:=
CC:=$(CROSS_COMPILER)g++
GCC:=$(CROSS_COMPILER)gcc
AR:=$(CROSS_COMPILER)ar

LIB_NAME:=./ini_parser.a
PROJECT_ROOT:=$(shell pwd)

INCLUDE_DIR+=-I$(PROJECT_ROOT)/include

CFLAGS+=$(INCLUDE_DIR)

all:$(LIB_NAME)
$(LIB_NAME):get_lan_string.o
	$(AR) -rsv -o $(LIB_NAME) get_lan_string.o
get_lan_string.o:$(PROJECT_ROOT)/get_lan_string.c
	$(GCC) $(CFLAGS) -c $(PROJECT_ROOT)/get_lan_string.c

clean:
	rm -f $(LIB_NAME)
	rm -f *.o 
