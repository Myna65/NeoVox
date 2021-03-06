#-------------------------------------------------
#
# Project created by QtCreator 2012-06-18T18:33:57
#
#-------------------------------------------------

QT       += core gui sql xml

TARGET = NeoVox
TEMPLATE = app
CONFIG += release


SOURCES += main.cpp\
        mainwindow.cpp \
    choixtestwindow.cpp \
    testwindow.cpp \
    resultatwindow.cpp \
    introwindow.cpp \
    optionswindow.cpp \
    about.cpp \
    welcome.cpp \
    language.cpp

HEADERS  += mainwindow.h \
    choixtestwindow.h \
    testwindow.h \
    resultatwindow.h \
    introwindow.h \
    optionswindow.h \
    about.h \
    welcome.h \
    language.h

FORMS    += mainwindow.ui \
    choixtestwindow.ui \
    testwindow.ui \
    resultatwindow.ui \
    introwindow.ui \
    optionswindow.ui \
    about.ui \
    welcome.ui \
    language.ui

RESOURCES += \
    Res.qrc

win32 {
RC_FILE += rc.rc
}

TRANSLATIONS += Vox_en.ts Vox_nl.ts Vox_es.ts Vox_de.ts Vox_la.ts Vox_el.ts
