#include <QtGui/QApplication>
#include <QTranslator>
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;

    //tell people what their locale is
    qDebug() << "current locale is " << QLocale::system().name();
    qDebug() << "\nThis will load witGuiLang_<locale text>.qm.  If that file is not found,\nit will try to load using only the first language characters\nand neglect the country characters";
    qDebug() << "\nie, if en_US is the locale variable, it will try to load witGuiLang_en_US.qm and then witGuiLang_en.qm.\nIf neither are found no language file is loaded and default values are used.";
    qDebug() << "\nqm files can be generated from the example .ts file using qt linguist\n";

    //load up a translation if it exists
    translator.load( QString( "witGuiLang_" ) + QLocale::system().name() );
    a.installTranslator( &translator );

    MainWindow w;
    w.show();
    return a.exec();
}
