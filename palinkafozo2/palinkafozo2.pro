#-------------------------------------------------
#
# Project created by QtCreator 2014-09-29T11:44:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = palinkafozo2
TEMPLATE = app


SOURCES += main.cpp\
        palinkafozo.cpp

HEADERS  += palinkafozo.h

FORMS    += palinkafozo.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../parapin/parapin-1.5.1-beta1/release/ -lparapin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../parapin/parapin-1.5.1-beta1/debug/ -lparapin
else:unix: LIBS += -L$$PWD/../parapin/parapin-1.5.1-beta1/ -lparapin

INCLUDEPATH += $$PWD/../parapin/parapin-1.5.1-beta1
DEPENDPATH += $$PWD/../parapin/parapin-1.5.1-beta1

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../parapin/parapin-1.5.1-beta1/release/libparapin.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../parapin/parapin-1.5.1-beta1/debug/libparapin.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../parapin/parapin-1.5.1-beta1/release/parapin.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../parapin/parapin-1.5.1-beta1/debug/parapin.lib
else:unix: PRE_TARGETDEPS += $$PWD/../parapin/parapin-1.5.1-beta1/libparapin.a
