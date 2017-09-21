TARGET = ellogso
TEMPLATE = lib
DEFINES += SO_LIBRARY

SOURCES += easylogging++.cc \
    ellog.cpp

HEADERS += \
    easylogging++.h \
    ellog.h

INCLUDEPATH += .

DISTFILES += \
    config.conf

unix {
    target.path = /usr/lib
    INSTALLS += target
}
