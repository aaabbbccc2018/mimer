TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lpthread
LIBS += -L./libs -lellog

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
    threadpool.h \
    SyncErrno.h

DEPENDPATH +=  ../../ellog
INCLUDEPATH +=  ../../ellog
