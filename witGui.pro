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

unix {
    system(chmod 755 ./makesvnrev.sh)
#    system(chmod 555 ./makesvnrev.sh)
#    system(chmod +x ./makesvnrev.sh)
    system(./makesvnrev.sh)
}
win32 {
    system(SubWCRev.exe "." "./svnrev_template.h" "./svnrev.h")
}

RESOURCES += resources.qrc
