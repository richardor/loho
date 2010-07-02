CROSS_COMPILER:=
CC:=$(CROSS_COMPILER)g++
GCC:=$(CROSS_COMPILER)gcc
AR:=$(CROSS_COMPILER)ar

EXE_NAME:=./fun_log_test


PROJECT_ROOT:=./


INCLUDE_DIR+=-I$(PROJECT_ROOT)/../include
STATIC_LIB+=../../release/libfunlog.a


CFLAGS+=-Wall  -Werror 
CFLAGS+=$(INCLUDE_DIR)
LDFLAGS+=$(STATIC_LIB)

all:$(EXE_NAME)
$(EXE_NAME): fun_log_test.o
	$(GCC) $(CFLAGS) -o $(EXE_NAME) fun_log_test.o $(LDFLAGS)
	@echo "ok"
fun_log_test.o:fun_log_test.c
	$(GCC) $(CFLAGS) -c $(PROJECT_ROOT)/fun_log_test.c $(LDFLAGS) 

clean:
	rm -f $(EXE_NAME)
	rm -f *.o



