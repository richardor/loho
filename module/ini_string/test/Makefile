CROSS_COMPILER:=
CC:=$(CROSS_COMPILER)g++
GCC:=$(CROSS_COMPILER)gcc
AR:=$(CROSS_COMPILER)ar

EXE_NAME:=./ini_string_test


PROJECT_ROOT:=./


INCLUDE_DIR+=-I$(PROJECT_ROOT)/../include
STATIC_LIB+=../ini_parser.a


CFLAGS+=-Wall  -Werror 
CFLAGS+=$(INCLUDE_DIR)
LDFLAGS+=$(STATIC_LIB)

all:$(EXE_NAME)
$(EXE_NAME): ini_string_test.o
	$(GCC) $(CFLAGS) -o $(EXE_NAME) ini_string_test.o $(LDFLAGS)
	@echo "ok"

ini_string_test.o:ini_string_test.c
	$(GCC) $(CFLAGS) -c $(PROJECT_ROOT)/ini_string_test.c $(LDFLAGS) 

clean:
	rm -f $(EXE_NAME)
	rm -f *.o
