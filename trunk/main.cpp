/************************************************************************************
*
*   - QtWitGui -				2010 giantpune
*
*   the multilingual, multiplatform, multiformat gui for messing with
*   Wii game images.
*
*   This software comes to you with a GPLv3 license.
*   http://www.gnu.org/licenses/gpl-3.0.html
*
*   Basically you are free to modify this code, distribute it, use it in
*   other projects, or anything along those lines.  Just make sure that any
*   derivative work gets the same license.  And don't remove this notice from
*   the derivative work.
*
*   And please, don't be a douche.  If you borrow code from here, don't claim
*   you wrote it.  Share your source code with others.  Even if you are
*   charging a fee for the binaries, let others read the code as somebody has
*   done for you.
*
*************************************************************************************/
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
    translator.load( QString( "language/witGuiLang_" ) + QLocale::system().name() );
    a.installTranslator( &translator );

    MainWindow w;
    w.show();
    return a.exec();
}
