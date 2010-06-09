# -------------------------------------------------
# Project created by QtCreator 2010-06-06T01:56:30
# -------------------------------------------------
QT += network
TARGET = witGui
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    filefolderdialog.cpp \
    wiitreethread.cpp
HEADERS += mainwindow.h \
    filefolderdialog.h \
    wiitreethread.h
FORMS += mainwindow.ui
TRANSLATIONS = witGuiTranslationExample.ts
VERSION = r$(shell svnversion .) # get svn revision
VERSTR = '\\"$${VERSION}\\"' # place quotes around the version string
DEFINES += SVNVER=\"$${VERSTR}\" # create a VER macro containing the version string
