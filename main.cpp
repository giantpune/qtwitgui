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

    QStringList args = QApplication::arguments();
    if( args.contains( "--greyness") )
    {
	QFile file( ":/darkTheme.qss" );
	file.open( QFile::ReadOnly );
	QString styleSheet = QLatin1String( file.readAll() );
	w.setStyleSheet( styleSheet );
    }
    w.show();

    int ret = a.exec();
    delete wiiTDB;
#ifdef Q_WS_WIN    //on windows XP, wmic leaves behind a temp file ( 0 bytes ): just delete it
    if( QFile::exists( "./TempWmicBatchFile.bat" ) )
        QFile::remove( "./TempWmicBatchFile.bat" );
#endif
    return ret;
}
