####################################################
# 
# 1）2010-6-7 初次创建此MAKEFILE
# 2) 2010-6-8 添加vio目录。
####################################################


#定义该平台上的交叉编译环境
CROSS_COMPILER:=arm-hismall-linux-
CC:=$(CROSS_COMPILER)g++
GCC:=$(CROSS_COMPILER)gcc

EXE_NAME:=./release/tzl_demo

#程序的目录结构####################################################
#根目录
PROJECT_ROOT:=$(shell pwd)/src
#发布程序目录
RELEASE_DIR:=$(PROJECT_ROOT)/release
#RMCOMMON目录
RMCOMMON_DIR:=$(PROJECT_ROOT)/rmcommon
#VIO目录
VIO_DIR:=$(PROJECT_ROOT)/vio
#ADCHIP目录
ADCHIP_DIR:=$(PROJECT_ROOT)/adchip


###############################################################
#定义编译选项
CFLAGS+=-Wall  -Werror 

LDFLAGS+=-lpthread -lm


#HI3520平台提供的头文件
INCLUDE_DIR+=-I$(PROJECT_ROOT)/hi3520/include
INCLUDE_DIR+=-I$(PROJECT_ROOT)/hi3520/include/mkp
INCLUDE_DIR_SLAVE+=-I$(PROJECT_ROOT)/hi3520/include
INCLUDE_DIR_SLAVE+=-I$(PROJECT_ROOT)/hi3520/include/mkp

#HI3520平台提供的库文件
HI3520STATIC_LIB_PATH:=$(PROJECT_ROOT)/hi3520/lib.rel
STATIC_LIB_LIST+=$(HI3520STATIC_LIB_PATH)/libmpi.a \
	$(HI3520STATIC_LIB_PATH)/lib_VoiceEngine.a \
 	$(HI3520STATIC_LIB_PATH)/lib_amr_spc.a \
	$(HI3520STATIC_LIB_PATH)/lib_amr_fipop.a \
	$(HI3520STATIC_LIB_PATH)/lib_aec.a \
	$(HI3520STATIC_LIB_PATH)/libloadbmp.a \
	$(HI3520STATIC_LIB_PATH)/libtde.a \
	$(HI3520STATIC_LIB_PATH)/lib_aacdec.a \
	$(HI3520STATIC_LIB_PATH)/lib_aacenc.a \
	$(HI3520STATIC_LIB_PATH)/libresampler.a \
	$(HI3520STATIC_LIB_PATH)/libpciv.a
	
#定义一些常用的宏定义和函数
INCLUDE_DIR+=-I$(RMCOMMON_DIR)/include		

#Video Input and Video Output
INCLUDE_DIR+=-I$(VIO_DIR)/include
	
#ADCHIP目录
INCLUDE_DIR+=-I$(ADCHIP_DIR)/include	
	
CFLAGS+=$(INCLUDE_DIR)
LDFLAGS+=$(STATIC_LIB_LIST)
	
objects = main.o vio.o adchip.o

$(EXE_NAME) : $(objects)
	$(CC) $(CFLAGS) -o $(EXE_NAME) $(objects) 	$(LDFLAGS)
main.o : $(PROJECT_ROOT)/main.c
	$(CC) $(CFLAGS) -c $(PROJECT_ROOT)/main.c  $(LDFLAGS)
vio.o : $(VIO_DIR)/vio.c
	$(CC) $(CFLAGS) -c $(VIO_DIR)/vio.c  $(LDFLAGS)
adchip.o : $(ADCHIP_DIR)/adchip.c
	$(CC) $(CFLAGS) -c $(ADCHIP_DIR)/adchip.c  $(LDFLAGS)
	

.PHONY : clean
clean:
	-rm -f $(EXE_NAME) *.o	



