#include "fsinfo.h"

#ifdef Q_WS_LINUX
#include <sys/vfs.h>
#define	    HFS_SUPER_MAGIC       0x4244
#define	    HPFS_SUPER_MAGIC      0xF995E849
#define	    MSDOS_SUPER_MAGIC     0x4d44
#define	    NTFS_SB_MAGIC         0x5346544e
#define	    NTFS_PUNE_MAGIC       0x65735546//all my NTFS return this ( created on XP x64 )
#endif

FsInfo::FsInfo(QObject *parent) :
    QObject(parent)
{
}

bool FsInfo::Check()
{
#ifndef Q_WS_WIN
    qDebug() << "FsInfo::Check() called in non-windows platform";
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

QString FsInfo::ToWinPath( QString cygPath, bool *ok )
{
    *ok = false;
#ifndef Q_WS_WIN
    qDebug() << "FsInfo::ToWinPath() called in non-windows platform";
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
    //qDebug() << "FsInfo::ToWinPath:" << cygPath << output;
    *ok = true;
    return output;
}

QString FsInfo::ToCygPath( QString winPath, bool *ok )
{
    *ok = false;
#ifndef Q_WS_WIN
    qDebug() << "FsInfo::ToCygPath() called in non-windows platform";
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
    //qDebug() << "FsInfo::ToCygPath:" << winPath << output;
    *ok = true;
    return output;
}

QString FsInfo::GetFilesystem( QString path )
{
    QProcess p;
#ifdef Q_WS_WIN
    bool ok = false;
    QString drive = ToWinPath( path, &ok );
    int colon = drive.indexOf( ":" );
    if( colon != 1 || !ok )
    {
	qDebug() << "FsInfo::GetFilesystem() colon != 1" << colon << ok;
	return QString();
    }
    drive.resize( 1 );

    p.start( "wmic", QStringList() << "/output:stdout" << "/interactive:off" << "/locale:ms_409" <<\
	     "logicaldisk" << "where" << "DeviceID=\'" + drive + ":\'" << "get" << "filesystem" );
#elif defined Q_WS_MAC
    QString command = "diskutil info " + path;
    //qDebug() << command;
    p.start( command );
#else
    //try statfs first as it is the fastest.  but its descriptors are less than descriptive for ext variants
    struct statfs fInfo;
    int r = statfs( path.toLatin1().data(), &fInfo );
    //qDebug() << "statfs::" << path.toLatin1().data() << r << "type:" << hex << fInfo.f_type;
    if( !r )
    {
	switch( fInfo.f_type )
	{
	case MSDOS_SUPER_MAGIC:
	    return "FAT";
	    break;
	case NTFS_SB_MAGIC:
	case NTFS_PUNE_MAGIC:
	    return "NTFS";
	    break;
	case HFS_SUPER_MAGIC:
	case HPFS_SUPER_MAGIC:
	    return "HPFS";
	    break;
	default:
	    break;
	}
    }
    QString command = "df -T " + path;
    //qDebug() << command;
    p.start( command );
#endif
    if( !p.waitForStarted( 5000 ) )
    {
	qDebug() << "FsInfo::GetFilesystem failed to start";
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
	qDebug() << "exit status ( getfs )" << p.exitCode();
	return QString();
    }

    QString output = p.readAll();
    output.remove( "\r" );
    QStringList list = output.split( "\n", QString::SkipEmptyParts );
#ifdef Q_WS_WIN
    if( !list.contains( "FileSystem  " ) || list.size() != 2 )
    {
	qDebug() << "wrong output ( getfs )" << list;
	return QString();
    }
    QString fs = list.at( 1 );
    fs = fs.simplified();
    return fs;
#elif defined Q_WS_MAC
    int size = list.size();
    for( int i = 0; i < size; i++ )
    {
	if( !list.at( i ).contains( "File System:" ) )
	    continue;

	QString fs = list.at( i );
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
#else
    int size = list.size();
    if( size != 2 )
    {
	qDebug() << "size != 2 ( getfs )" << list;
	return QString();
    }
    QString fs = list.at( 1 );
    fs = fs.simplified();
    if( fs.count( " " ) < 2 )
    {
	qDebug() << "spaces < 2 ( getfs )" << fs;
	return QString();
    }
    fs.remove( 0, fs.indexOf( " ") + 1 );
    fs.resize( fs.indexOf( " " ) );
    if( fs == "devtmpfs" )//this is what it shows if if has permission to read a block device -- ie,  WBFS partition at /dev/sdc2
    {
	qDebug() << "fs == devtmpfs ( getfs )";
	return QString();
    }
    return fs;
#endif
}

