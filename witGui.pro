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
TRANSLATIONS = language/witGuiLang_empty.ts \
    language/witGuiLang_nl.ts \
    language/witGuiLang_fr.ts \
    language/witGuiLang_ja.ts \
    language/witGuiLang_da.ts \
    language/witGuiLang_it.ts \
    language/witGuiLang_es.ts
#create new svnrev.h
unix {
    system(chmod 755 ./makesvnrev.sh)
    system(./makesvnrev.sh)
}
win32 {
    system(SubWCRev.exe "." "./svnrev_template.h" "./svnrev.h")
}

#update language files (.ts)
system(lupdate -verbose ./witGui.pro)

#turn .ts into .qm files.  strip identical strings
system(lrelease -verbose ./witGui.pro)

RESOURCES += resources.qrc
