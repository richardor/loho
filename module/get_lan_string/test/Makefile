CROSS_COMPILER:=
CC:=$(CROSS_COMPILER)g++
GCC:=$(CROSS_COMPILER)gcc
AR:=$(CROSS_COMPILER)ar

EXE_NAME:=./get_string_test


PROJECT_TEST_ROOT:=./
PROJECT_SRC_ROOT:=./../src


INCLUDE_DIR+=-I$(PROJECT_SRC_ROOT)/include
STATIC_LIB+=


CFLAGS+=-Wall  -Werror 
CFLAGS+=$(INCLUDE_DIR)
LDFLAGS+=$(STATIC_LIB)

all:$(EXE_NAME)
$(EXE_NAME): test_get_lan_string.o get_lan_string.o
	$(GCC) $(CFLAGS) -o $(EXE_NAME) test_get_lan_string.o get_lan_string.o $(LDFLAGS)
	@echo "ok"
get_lan_string.o : $(PROJECT_SRC_ROOT)/get_lan_string.c
	$(GCC) $(CFLAGS) -c $(PROJECT_SRC_ROOT)/get_lan_string.c $(LDFLAGS) 
test_get_lan_string.o : test_get_lan_string.c
	$(GCC) $(CFLAGS) -c $(PROJECT_TEST_ROOT)/test_get_lan_string.c $(LDFLAGS) 
	
clean:
	rm -f $(EXE_NAME)
	rm -f *.o



