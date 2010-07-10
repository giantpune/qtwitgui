# -------------------------------------------------
# Project created by QtCreator 2010-06-06T01:56:30
# -------------------------------------------------
#QT += network
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
    language/witGuiLang_es.ts \
    language/witGuiLang_es.ts
#create new svnrev.h
unix {
    system( chmod 755 ./makesvnrev.sh )
    system( ./makesvnrev.sh )
    macx{
#MAC icon & universal binary
	RC_FILE = icon.icns
	CONFIG += x86 ppc
    }
}
win32 {
    system(SubWCRev.exe "." "./svnrev_template.h" "./svnrev.h")
    RC_FILE = rcfile.rc
}


system(lupdate -verbose ./witGui.pro)
system(lrelease -verbose ./witGui.pro)

RESOURCES += resources.qrc
