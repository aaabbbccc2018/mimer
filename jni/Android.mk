#获取当前目录
LOCAL_PATH := $(call my-dir)
#清除一些变量
include $(CLEAR_VARS)
#要生成的库名
LOCAL_MODULE    := libmqtter
#指定平台
#LOCAL_ARM_MODE := arm
#需要引用的库
LOCAL_LDFLAGS  := -fPIC -shared  -Wl -ldl -m
LOCAL_CPPFLAGS += -fexceptions -frtti
#-L$(SYSROOT)/usr/lib -lrt -luuid -pthread
#编译参数
LOCAL_CFLAGS := -Wall -O3 -enable-threads
#定义宏
TOP_INCLUDE := $(LOCAL_PATH)/../MQTTProtocol
LOCAL_C_INCLUDES := $(TOP_INCLUDE)

$(warning "include" $(TOP_INCLUDE) $(LOCAL_C_INCLUDES))
TOP_SRC := $(LOCAL_PATH)/../MQTTProtocol
#库对应的源文件
LOCAL_SRC_FILES := $(TOP_SRC)/MQTTInt.cpp \
	$(TOP_SRC)/MQTTPacket.cpp             \
	$(TOP_SRC)/MQTTProtocol.cpp           \
	$(TOP_SRC)/Stream.cpp

$(warning "src" $(LOCAL_SRC_FILES))
#生成共享库
include $(BUILD_SHARED_LIBRARY)
