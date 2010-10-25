#include <QtGui/QApplication>
#include "mainwindow.h"
#include "includes.h"
#include "wiitdb.h"
#include "tools.h"


/*
  QPalette ( const QBrush & windowText,
const QBrush & button,
const QBrush & light,
const QBrush & dark,
const QBrush & mid,
const QBrush & text,
const QBrush & bright_text,
const QBrush & base,
const QBrush & window )
  */
/*
  fg_color:#d7d7d7,
bg_color:#242424,
base_color:#474747,
text_color:#d7d7d7,
selected_bg_color:#d7d7d7,
selected_fg_color:#3d3d3d,
tooltip_bg_color:#d7d7d7,
tooltip_fg_color:#3d3d3d
      */
int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE( resources );

    QApplication a(argc, argv);

    wiiTDB = new WiiTDB;
    //qDebug() << PROGRAM_NAME;

    QApplication::setWindowIcon( QIcon( ":images/icon.ico" ) );

    QFile file( ":/darkTheme.qss" );
    file.open( QFile::ReadOnly );
    QString styleSheet = QLatin1String( file.readAll() );
    //setStyleSheet( styleSheet );

    //QPalette p = QApplication::palette();
    /*QBrush c1( QColor( "#242424" ) );//dark
    QBrush c3( QColor( "#3d3d3d" ) );
    QBrush c4( QColor( "#474747" ) );
    QBrush c2( QColor( "#d7d7d7" ) );//light

    //TODO: palette still nowhere complete
    QPalette newPalette( c2, c4, c4, c1, c3, c2, c4, c4, c4  );

    QApplication::setPalette( newPalette );*/
    settingsPath = QDesktopServices::storageLocation( QDesktopServices::HomeLocation ) + "/QtWitGui.ini";
    MainWindow w;
    w.setStyleSheet( styleSheet );
    //w.setWindowIcon( QIcon( ":images/icon.ico" ) );
    w.show();

    int ret = a.exec();
    delete wiiTDB;
#ifdef Q_WS_WIN    //on windows XP, wmic leaves behind a temp file ( 0 bytes ): just delete it
    if( QFile::exists( "./TempWmicBatchFile.bat" ) )
        QFile::remove( "./TempWmicBatchFile.bat" );
#endif
    return ret;
}
