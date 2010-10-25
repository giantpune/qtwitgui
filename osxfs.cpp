#include "osxfs.h"

OSxFs::OSxFs(QObject *parent ) : QObject( parent )
{
}
QString OSxFs::GetFilesystem( QString path )
{
#ifndef Q_WS_MAC
    qDebug() << "OSx::GetFilesystem() called in non-MAC platform";
    return QString();
#endif
    QProcess process;
    QString command = "diskutil info " + path;
    //qDebug() << command;
    process.start( command );
    if( !process.waitForStarted() )//default timeout 30,000 msecs
    {
        qDebug() << "OSxFs::GetFilesystem  failed to start";
        return QString();
    }
    process.closeWriteChannel();

    if( !process.waitForFinished() )
    {
        qDebug() << "!process.waitForFinished() ( OSxFs::GetFilesystem )";
        return QString();
    }

    QString output = process.readAll();
    QStringList lines = output.split( "\n", QString::SkipEmptyParts );
    int size = lines.size();
    for( int i = 0; i < size; i++ )
    {
        if( !lines.at( i ).contains( "File System:" ) )
            continue;

        QString fs = lines.at( i );
        fs.remove( 0, fs.indexOf( "File System:" ) + 12 );
        fs = fs.trimmed();
        int space = fs.indexOf( " " );
        if( space > 0 )
        {
            fs.remove( 0, space + 1 );
        }
        //qDebug() << fs;
        return fs;
    }

    return  QString();
}
