TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    SyncTools.cpp
#    stdcpp/SDL_syscond.cpp \
#    stdcpp/SDL_sysmutex.cpp \
#    stdcpp/SDL_systhread.cpp \
#    generic/SDL_syscond.c \
#    generic/SDL_sysmutex.c \
#    generic/SDL_syssem.c \
#    generic/SDL_systhread.c \
#    generic/SDL_systls.c \
#    psp/SDL_syscond.c \
#    psp/SDL_sysmutex.c \
#    psp/SDL_syssem.c \
#    psp/SDL_systhread.c \
#    pthread/SDL_syscond.c \
#    pthread/SDL_sysmutex.c \
#    pthread/SDL_syssem.c \
#    pthread/SDL_systhread.c \
#    pthread/SDL_systls.c \
#    windows/SDL_sysmutex.c \
#    windows/SDL_syssem.c \
#    windows/SDL_systhread.c \
#    windows/SDL_systls.c \
#    SDL_thread.c \

HEADERS += \
#    generic/SDL_sysmutex_c.h \
#    generic/SDL_systhread_c.h \
#    psp/SDL_sysmutex_c.h \
#    psp/SDL_systhread_c.h \
#    pthread/SDL_sysmutex_c.h \
#    pthread/SDL_systhread_c.h \
#    stdcpp/SDL_sysmutex_c.h \
#    stdcpp/SDL_systhread_c.h \
#    windows/SDL_systhread_c.h \
#    SDL_systhread.h \
#    SDL_thread_c.h \
    SyncTools.h \
    SyncType.h
