# -------------------------------------------------
# Project created by QtCreator 2010-06-06T01:56:30
# -------------------------------------------------
QT += network
TARGET = QtWitGui
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    filefolderdialog.cpp \
    wiitreethread.cpp
HEADERS += mainwindow.h \
    filefolderdialog.h \
    wiitreethread.h
FORMS += mainwindow.ui
TRANSLATIONS = language/witGuiTranslationExample.ts \
    language/witGuiTranslation_nl_NL.ts \
    language/witGuiTranslation_fr_FR.ts \
    language/witGuiTranslation_ja_JP.ts \
    language/witGuiTranslation_da_DK.ts
VERSION = r$(shell svnversion .) # get svn revision
VERSTR = '\\"$${VERSION}\\"' # place quotes around the version string
DEFINES += SVNVER=\"$${VERSTR}\" # create a VER macro containing the version string
RESOURCES += resources.qrc
