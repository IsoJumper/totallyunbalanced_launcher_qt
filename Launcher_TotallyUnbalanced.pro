#-------------------------------------------------
#
# Project created by QtCreator 2015-09-15T15:55:31
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11


### COMMENT THE FOLLOWING IF OPENSSL NOT INSTALLED ###
DEFINES += USE_OPENSSL_MD5
LIBS += -lcrypto
######################################################

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Launcher_TotallyUnbalanced
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ssettings.cpp \
    startupcheck.cpp \
    checksum.cpp

HEADERS  += mainwindow.h \
    ssettings.h \
    startupcheck.h \
    checksum.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
