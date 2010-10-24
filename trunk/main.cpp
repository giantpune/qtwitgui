#include <QtGui/QApplication>
#include "mainwindow.h"
#include "includes.h"
#include "wiitdb.h"
#include "tools.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE( resources );

    QApplication a(argc, argv);

    wiiTDB = new WiiTDB;
    //qDebug() << PROGRAM_NAME;

    QApplication::setWindowIcon( QIcon( ":images/icon.ico" ) );
    settingsPath = QDesktopServices::storageLocation( QDesktopServices::HomeLocation ) + "/QtWitGui.ini";
    MainWindow w;
    //w.setWindowIcon( QIcon( ":images/icon.ico" ) );
    w.show();

    int ret = a.exec();
    delete wiiTDB;
    return ret;
}
