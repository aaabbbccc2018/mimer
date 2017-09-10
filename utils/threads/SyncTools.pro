TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lpthread

SOURCES += main.cpp \
    SyncTools.cpp \
    threads.cpp \
    utilException.cpp \
    threadpool.cpp

HEADERS += \
    SyncTools.h \
    SyncType.h \
    threads.h \
    utilException.h \
    threadpool.h
