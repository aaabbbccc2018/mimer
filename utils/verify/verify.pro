TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    crc.cpp \
    md5.cpp \
    sha1.cpp

HEADERS += \
    crc.h \
    md5.h \
    sha1.h \
    verify.h \
    base64.h
