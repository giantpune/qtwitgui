#include "windowsfsstuff.h"

WindowsFsStuff::WindowsFsStuff(QObject *parent) :
    QObject(parent)
{
}

bool WindowsFsStuff::Check()
{
#ifndef Q_WS_WIN
    qDebug() << "WindowsFsStuff::Check() called in non-windows platform";
    return false;
#endif
    QProcess p;

    //check that we have the program to convert windows paths to proper cygwin paths
    p.start( "cygpath -v" );
    if( !p.waitForStarted() )
    {
        qDebug() << "failed to start cygpath ( check )";
        return false;
    }

    if( !p.waitForFinished() )
    {
        qDebug() << "!p.waitForFinished() ( winfs stuff )";
        return false;
    }
    if( p.exitCode() != QProcess::NormalExit )
    {
        qDebug() << "exit status ( winfs stuff )" << p.exitCode() << QProcess::NormalExit;
        return false;
    }

    QString output = p.readAll();
    if( !output.contains( "Cygwin pathconv" ) )
    {
        qDebug() << "cygpath text output wrong ( winfs stuff ):" << output;
        return false;
    }

    //check that we can access wmic to query the filesystems
    //just query the operating system for a test
    p.start( "wmic", QStringList() << "/output:stdout" << "/interactive:off" << "/locale:ms_409" <<\
             "OS" << "GET" << "caption" );
    if( !p.waitForStarted() )
    {
        qDebug() << "failed to start wmic ( check )";
        return false;
    }
    p.closeWriteChannel();

    if( !p.waitForFinished() )
    {
        qDebug() << "!p.waitForFinished() wmic ( winfs stuff )";
        return false;
    }
    if( p.exitCode() != QProcess::NormalExit )
    {
        qDebug() << "exit status wmic ( winfs stuff )";
        return false;
    }

    output = p.readAll();
    if( !output.contains( "Windows" ) )
    {
        qDebug() << "wmic text output wrong ( winfs stuff ):" << output;
        return false;
    }
    return true;
}

QString WindowsFsStuff::ToWinPath( QString cygPath, bool *ok )
{
    *ok = false;
#ifndef Q_WS_WIN
    qDebug() << "WindowsFsStuff::ToWinPath() called in non-windows platform";
    return QString();
#endif
    QProcess p;
    p.start( "cygpath", QStringList() << "-w" << cygPath );
    if( !p.waitForStarted() )
    {
        qDebug() << "failed to start cygpath ( towinpath )";
        return QString();
    }

    if( !p.waitForFinished() )
    {
        qDebug() << "!p.waitForFinished() ( winfs stuff )";
        return QString();
    }
    if( p.exitCode() != QProcess::NormalExit )
    {
        qDebug() << "exit status ( winfs stuff )";
        return QString();
    }

    QString output = p.readAll();
    output.remove( "\r" );
    output.remove( "\n" );
    //qDebug() << "WindowsFsStuff::ToWinPath:" << cygPath << output;
    *ok = true;
    return output;
}

QString WindowsFsStuff::ToCygPath( QString winPath, bool *ok )
{
    *ok = false;
#ifndef Q_WS_WIN
    qDebug() << "WindowsFsStuff::ToCygPath() called in non-windows platform";
    return QString();
#endif
    QProcess p;
    p.start( "cygpath", QStringList() << "-u" << winPath );
    if( !p.waitForStarted() )
    {
        qDebug() << "failed to start cygpath ( tocygpath )";
        return QString();
    }

    if( !p.waitForFinished() )
    {
        qDebug() << "!p.waitForFinished() ( winfs stuff )";
        return QString();
    }
    if( p.exitCode() != QProcess::NormalExit )
    {
        qDebug() << "exit status ( winfs stuff )";
        return QString();
    }

    QString output = p.readAll();
    output.remove( "\r" );
    output.remove( "\n" );
    //qDebug() << "WindowsFsStuff::ToCygPath:" << winPath << output;
    *ok = true;
    return output;
}

QString WindowsFsStuff::GetFilesystem( QString path )
{
#ifndef Q_WS_WIN
    qDebug() << "WindowsFsStuff::GetFilesystem() called in non-windows platform";
    return QString();
#endif
    QProcess p;

    bool ok = false;
    QString drive = ToWinPath( path, &ok );
    int colon = drive.indexOf( ":" );
    if( colon != 1 || !ok )
    {
        qDebug() << "WindowsFsStuff::GetFilesystem() colon != 1" << colon << ok;
        return QString();
    }
    drive.resize( 1 );

    //check that we have the program to convert windows paths to proper cygwin paths
    p.start( "wmic", QStringList() << "/output:stdout" << "/interactive:off" << "/locale:ms_409" <<\
             "logicaldisk" << "where" << "DeviceID=\'" + drive + ":\'" << "get" << "filesystem" );
    if( !p.waitForStarted() )
    {
        qDebug() << "failed to start wmic ( getfs )";
        return QString();
    }
    p.closeWriteChannel();

    if( !p.waitForFinished() )
    {
        qDebug() << "!p.waitForFinished() ( getfs )";
        return QString();
    }
    if( p.exitCode() != QProcess::NormalExit )
    {
        qDebug() << "exit status ( getfs )" << p.exitCode() << QProcess::NormalExit;
        return QString();
    }

    QString output = p.readAll();
    output.remove( "\r" );
    QStringList list = output.split( "\n", QString::SkipEmptyParts );
    if( !list.contains( "FileSystem  " ) || list.size() != 2 )
    {
        qDebug() << "wrong output ( getfs )" << list;
        return QString();
    }
    QString fs = list.at( 1 );
    fs = fs.simplified();
    return fs;
}
