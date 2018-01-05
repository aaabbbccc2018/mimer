#获取当前目录
LOCAL_PATH := $(call my-dir)
#清除一些变量
include $(CLEAR_VARS)
#要生成的库名
LOCAL_MODULE    := libtransmitter
#指定平台
#LOCAL_ARM_MODE := arm
#需要引用的库
LOCAL_LDFLAGS  := -fPIC -shared -ldl
ifdef ($(LIBROOT))
LOCAL_LDFLAGS += -L $(LIBROOT)/ellog/$(TARGET_ARCH_ABI) -lellog
LOCAL_LDFLAGS += -L $(LIBROOT)/uvbase/$(TARGET_ARCH_ABI) -luvbase
LOCAL_LDFLAGS += -L $(LIBROOT)/mimp/$(TARGET_ARCH_ABI) -lmimp
LOCAL_LDFLAGS += -L $(LIBROOT)/uv/$(TARGET_ARCH_ABI) -luv
else
LOCAL_LDFLAGS += -L ../../../../../lib/android/ellog/$(TARGET_ARCH_ABI) -lellog
LOCAL_LDFLAGS += -L ../../../../../lib/android/uvbase/$(TARGET_ARCH_ABI) -luvbase
LOCAL_LDFLAGS += -L ../../../../../lib/android/mimp/$(TARGET_ARCH_ABI) -lmimp
LOCAL_LDFLAGS += -L ../../../../../lib/android/uv/$(TARGET_ARCH_ABI) -luv
endif
LOCAL_LDLIBS += -latomic
LOCAL_CPPFLAGS += -fexceptions -frtti
#-L$(SYSROOT)/usr/lib -lrt -luuid -pthread
#编译参数
LOCAL_CFLAGS := -Wall -O3 -enable-threads
#定义宏
TOP_INCLUDE := $(LOCAL_PATH)/
LOCAL_C_INCLUDES := $(TOP_INCLUDE)../ \
	$(TOP_INCLUDE)/../../../../uvbase        \
	$(TOP_INCLUDE)/../../../../libuv/include \
	$(TOP_INCLUDE)/../../../../../ellog      \
	$(TOP_INCLUDE)/../../../../../MIMProtocol
$(warning "include"  $(LOCAL_C_INCLUDES))
TOP_SRC := $(LOCAL_PATH)/../
#库对应的源文件
MY_CPP_LIST := $(wildcard $(TOP_SRC)/*.cpp)
$(warning "source" $(MY_CPP_LIST))
LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

#生成共享库
include $(BUILD_SHARED_LIBRARY)
