TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    MQTTProtocol.cpp \
    MQTTPacket.cpp \
    MQTTInt.cpp \
    List.cpp \
    Stream.cpp

HEADERS += \
    MQTTProtocol.h \
    MQTTPacket.h \
    MQTTInt.h \
    List.h \
    Stream.h
