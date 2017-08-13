TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
DEFINES += REVERSED=0

SOURCES += main.cpp \
    MIMProtocol.cpp \
    MIMPacket.cpp \
    MIMInt.cpp \
    List.cpp \
    Stream.cpp \
    test.cpp \
    memUtil.c \
    timeUtil.c

HEADERS += \
    MIMProtocol.h \
    MIMPacket.h \
    MIMInt.h \
    List.h \
    Stream.h \
    MIMErr.h \
    memUtil.h \
    version.h \
    timeUtil.h
