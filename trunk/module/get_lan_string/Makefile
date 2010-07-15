CC:=g++
GCC:=gcc
AR:=ar

LIB_NAME:=./release/libfunlog.a
PROJECT_ROOT:=$(shell pwd)/src

INCLUDE_DIR+=-I$(PROJECT_ROOT)/	



CFLAGS+=$(INCLUDE_DIR)

all:$(LIB_NAME)
$(LIB_NAME):fun_log.o
	$(AR) -r $(LIB_NAME) fun_log.o
	@echo "ok"
	@rm -f *.bak *.o
fun_log.o:$(PROJECT_ROOT)/fun_log.c
	$(GCC) $(CFLAGS) -c $(PROJECT_ROOT)/fun_log.c 
 
clean:
	rm -f $(LIB_NAME)
	rm -f *.o 
