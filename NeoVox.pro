#-------------------------------------------------
#
# Project created by QtCreator 2012-06-18T18:33:57
#
#-------------------------------------------------

QT       += core gui sql

TARGET = VOX
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    choixtestwindow.cpp \
    testwindow.cpp \
    resultatwindow.cpp \
    introwindow.cpp \
    optionswindow.cpp \
    about.cpp \
    welcome.cpp

HEADERS  += mainwindow.h \
    choixtestwindow.h \
    testwindow.h \
    resultatwindow.h \
    introwindow.h \
    optionswindow.h \
    about.h \
    welcome.h

FORMS    += mainwindow.ui \
    choixtestwindow.ui \
    testwindow.ui \
    resultatwindow.ui \
    introwindow.ui \
    optionswindow.ui \
    about.ui \
    welcome.ui

RESOURCES += \
    Res.qrc

TRANSLATIONS += Vox_en.ts
