#-------------------------------------------------
#
# Project created by QtCreator 2015-09-01T11:42:43
#
#-------------------------------------------------

QT       += widgets
QT       += core
QT       += gui

TARGET = cgds
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    aref.cpp \
    boundary.cpp \
    canvas.cpp \
    elements.cpp \
    gdsio.cpp \
    graphicsitems.cpp \
    library.cpp \
    path.cpp \
    sref.cpp \
    structures.cpp \
    techfile.cpp \
    text.cpp

HEADERS += \
    aref.h \
    boundary.h \
    canvas.h \
    elements.h \
    gdsio.h \
    graphicsitems.h \
    library.h \
    path.h \
    sref.h \
    structures.h \
    tags.h \
    techfile.h \
    text.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
