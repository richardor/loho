CROSS_COMPILER:=
CC:=$(CROSS_COMPILER)g++
GCC:=$(CROSS_COMPILER)gcc
AR:=$(CROSS_COMPILER)ar

LIB_NAME:=./release/ini_parser.a
PROJECT_ROOT:=$(shell pwd)/src

INCLUDE_DIR+=-I$(PROJECT_ROOT)/include

#CFLAGS+=-Wall  -Werror

CFLAGS+=$(INCLUDE_DIR)

all:$(LIB_NAME)
$(LIB_NAME):ini_parser.o
	$(AR) -rsv -o $(LIB_NAME) ini_parser.o
ini_parser.o:$(PROJECT_ROOT)/ini_parser.c
	$(CC) $(CFLAGS) -c $(PROJECT_ROOT)/ini_parser.c
 
clean:
	rm -f $(LIB_NAME)
	rm -f *.o 
