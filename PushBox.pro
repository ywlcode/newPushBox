# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

QT       += core gui multimedia sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets 
CONFIG += c++11 resources_big

TEMPLATE = app
TARGET = PushBox
DESTDIR = ./Debug
CONFIG += debug
LIBS += -L"."
DEPENDPATH += .
MOC_DIR += .
OBJECTS_DIR += debug
UI_DIR += .
RCC_DIR += .
HEADERS += ./beginpage.h \
    ./gamepage.h \
    ./leveldata.h
SOURCES += ./main.cpp \
    ./beginpage.cpp \
    ./gamepage.cpp \
    ./leveldata.cpp
FORMS += ./PushBox.ui \
    ./beginpage.ui \
    ./gamepage.ui
RESOURCES += res.qrc
QMAKE_CXXFLAGS += /utf-8

DISTFILES += \
    logo.rc
RC_FILE += logo.rc
