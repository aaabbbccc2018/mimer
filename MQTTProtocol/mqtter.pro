TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
DEFINES += REVERSED=0

SOURCES += main.cpp \
    MQTTProtocol.cpp \
    MQTTPacket.cpp \
    MQTTInt.cpp \
    List.cpp \
    Stream.cpp \
    test.cpp

HEADERS += \
    MQTTProtocol.h \
    MQTTPacket.h \
    MQTTInt.h \
    List.h \
    Stream.h \
    MQErr.h
