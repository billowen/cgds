#-------------------------------------------------
#
# Project created by QtCreator 2015-09-01T11:48:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app

SOURCES += main.cpp
CONFIG += c++11


HEADERS  +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../cgds/release/ -lcgds
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../cgds/debug/ -lcgds
else:unix: LIBS += -L$$OUT_PWD/../cgds/ -lcgds

INCLUDEPATH += $$PWD/../cgds
DEPENDPATH += $$PWD/../cgds

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../cgds/release/libcgds.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../cgds/debug/libcgds.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../cgds/release/cgds.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../cgds/debug/cgds.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../cgds/libcgds.a
