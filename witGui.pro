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
    unixfschecker.cpp \
    tools.cpp \
    gc_shrinkthread.cpp \
    filefolderdialog.cpp \
    gamewindow.cpp \
    wiitreethread.cpp \
    unzip/unzip.cpp \
    savedialog.cpp \
    covermanagerwindow.cpp \
    coverloaderthread.cpp \
    aboutdialog.cpp \
    windowsfsstuff.cpp
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
    unixfschecker.h \
    tools.h \
    gc_shrinkthread.h \
    filefolderdialog.h \
    gamewindow.h \
    wiitreethread.h \
    unzip/zip_p.h \
    unzip/zipentry_p.h \
    unzip/unzip_p.h \
    unzip/unzip.h \
    unzip/zlib.h \
    unzip/zconf.h \
    savedialog.h \
    covermanagerwindow.h \
    coverloaderthread.h \
    aboutdialog.h \
    windowsfsstuff.h
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
    aboutdialog.ui
RESOURCES += resources.qrc
MOC_DIR = obj
OBJECTS_DIR = obj

# create new svnrev.h
unix { 
    system( chmod 755 ./makesvnrev.sh )
    system( ./makesvnrev.sh )
    macx { 
        # MAC icon & universal binary
        RC_FILE = icon.icns
        CONFIG += x86 \
            ppc
    }
}
win32 { 
    system(SubWCRev.exe "." "./svnrev_template.h" "./svnrev.h")
    RC_FILE = rcfile.rc
}
OTHER_FILES += darkTheme.qss
