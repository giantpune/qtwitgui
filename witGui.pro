# -------------------------------------------------
# Project created by QtCreator 2010-10-04T22:54:43
# -------------------------------------------------
QT += core \
    gui \
    xml \
    network
TARGET = QtWitGui
TEMPLATE = app
DEFINES += PROGRAM_NAME='\\"${TARGET}\\"'
!win32:!macx: DEFINES += Q_WS_LINUX
SOURCES += main.cpp \
    mainwindow.cpp \
    pictureflow.cpp \
    wiitdb.cpp \
    hddselectdialog.cpp \
    settingsdialog.cpp \
    partitionwindow.cpp \
    withandler.cpp \
    passworddialog.cpp \
    wwthandler.cpp \
    wiitdbwindow.cpp \
    custommdiitem.cpp \
    gamecopydialog.cpp \
    tools.cpp \
    gc_shrinkthread.cpp \
    filefolderdialog.cpp \
    gamewindow.cpp \
    wiitreethread.cpp \
    savedialog.cpp \
    covermanagerwindow.cpp \
    coverloaderthread.cpp \
    aboutdialog.cpp \
 #   unixfschecker.cpp \
 #   osxfs.cpp \
 #   windowsfsstuff.cpp \
    fsinfo.cpp \
    formatwindow.cpp
!macx{
    SOURCES += unzip/unzip.cpp
}
HEADERS += mainwindow.h \
    pictureflow.h \
    wiitdb.h \
    hddselectdialog.h \
    settingsdialog.h \
    partitionwindow.h \
    withandler.h \
    passworddialog.h \
    wwthandler.h \
    wiitdbwindow.h \
    custommdiitem.h \
    includes.h \
    gamecopydialog.h \
    tools.h \
    gc_shrinkthread.h \
    filefolderdialog.h \
    gamewindow.h \
    wiitreethread.h \
    savedialog.h \
    covermanagerwindow.h \
    coverloaderthread.h \
    aboutdialog.h \
 #   unixfschecker.h \
 #   osxfs.h \
 #   windowsfsstuff.h \
    fsinfo.h \
    formatwindow.h
!macx{
    HEADERS += unzip/zip_p.h \
    unzip/zipentry_p.h \
    unzip/unzip_p.h \
    unzip/unzip.h \
    unzip/zlib.h \
    unzip/zconf.h
}
FORMS += mainwindow.ui \
    hddselectdialog.ui \
    settingsdialog.ui \
    partitionwindow.ui \
    passworddialog.ui \
    wiitdbwindow.ui \
    gamecopydialog.ui \
    gamewindow.ui \
    savedialog.ui \
    covermanagerwindow.ui \
    aboutdialog.ui \
    formatwindow.ui
RESOURCES += resources.qrc
MOC_DIR = obj
OBJECTS_DIR = obj

# create new svnrev.h
unix {
    system( chmod 755 ./makesvnrev.sh )
    system( ./makesvnrev.sh )
    macx {
        # MAC icon & universal binary
        RC_FILE = images/icon.icns
#        CONFIG += x86 ppc      #TODO: something is fucked up with my version of Qt, and this doesnt work out too well
    }
}
win32 {
    system(SubWCRev.exe "." "./svnrev_template.h" "./svnrev.h")
    RC_FILE = rcfile.rc
}
OTHER_FILES += darkTheme.qss

TRANSLATIONS = language/witGuiLang_empty.ts \
    language/witGuiLang_nl.ts \
    language/witGuiLang_fr.ts \
    language/witGuiLang_ja.ts \
    language/witGuiLang_da.ts \
    language/witGuiLang_it.ts \
    language/witGuiLang_es.ts \
    language/witGuiLang_de.ts

#system( lupdate ./witGui.pro )
#system( lrelease ./witGui.pro )
