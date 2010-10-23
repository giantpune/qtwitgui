#include "unixfschecker.h"
#include "includes.h"
#include "tools.h"

extern QString rootPass;
extern QString rootAskStr;
extern QString rootWrongStr;
extern QString rootFailStr;


UnixFsChecker::UnixFsChecker( QObject *parent ) :QObject( parent )
{
#ifdef Q_WS_WIN
    qDebug() << "UnixFsChecker::UnixFsChecker winblows warning";
    return;
#endif
    runAsRoot = false;
    running = false;

    //create the pointer to the process used to run file
    process = new QProcess( this );

    //and also send a "kill" message if this object is destroyed while the process is running
    connect( process, SIGNAL( readyReadStandardError() ), this, SLOT( ReadyReadStdErrSlot() ) );
    connect( process, SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( ProcessFinishedSlot(  int, QProcess::ExitStatus ) ) );
    connect( this, SIGNAL( KillProcess() ), process, SLOT( kill() ) );
}

UnixFsChecker::~UnixFsChecker()
{
    if( running )
    {
	emit KillProcess();
    }
    delete process;
    process = 0;
}

void UnixFsChecker::SetRunAsRoot( bool root )
{
    runAsRoot = root;
}

void UnixFsChecker::ReadyReadStdErrSlot()
{
    //read text from sudo / file
    errStr += process->readAllStandardError();

    qDebug() << "file: gotmessage err" << errStr;

    if( errStr.startsWith( rootWrongStr ) )
    {
	errStr.clear();
	rootPass.clear();
    }

    if( errStr.startsWith( rootAskStr ) )
    {
	errStr.clear();
	emit RequestPassword();
	return;
    }
    if( errStr == rootFailStr )
    {
	emit KillProcess();
	process->waitForFinished();//force kill the thing
	qDebug() << "killed sudo";
	emit SendFatalErr( errStr, UNIX_FS_CHECKER );
	errStr.clear();
	rootPass.clear();
	running = false;
    }
}

void UnixFsChecker::ProcessFinishedSlot( int i, QProcess::ExitStatus s )
{
    //qDebug() << "UnixFsChecker::ProcessFinishedSlot" << i << s;
    running = false;
    if( i || s )
    {
	if( s )
	{
	    emit SendFatalErr( tr( "file appears to have crashed" ), UNIX_FS_CHECKER );
	    return;
	}

	if( !errStr.isEmpty() )
	{
	    emit SendFatalErr( errStr, UNIX_FS_CHECKER );
	    return;
	}

	//clear old errors
	errStr.clear();
    }

    QStringList ret;
    QString stdStr = process->readAllStandardOutput();
    QStringList mounts = stdStr.split( "\n", QString::SkipEmptyParts );
    int size = partsResolved.size();
    int size2 = mounts.size();
    for( int i = 0; i < size; i++ )
    {
	for( int j = 0; j < size2; j++ )
	{
	    if( mounts.at( j ).contains( partsResolved.at( i ) ) )
	    {
		ret << partsMounted.at( i );
		QString fullDesc = mounts.at( j );
		fullDesc.remove( 0, partsResolved.at( i ).size() );//TODO:  add more known to this list.  these are all i have available, so its good enough for now
		if( fullDesc.contains( "OEM-ID \"NTFS    \"" ) )
		{
		    ret << "NTFS";
		    break;
		}
		if( fullDesc.contains( "Linux rev 1.0 ext3" ) )
		{
		    ret << "EXT3";
		    break;
		}
		if( fullDesc.contains( "Linux rev 1.0 ext4" ) )
		{
		    ret << "EXT4";
		    break;
		}
		if( fullDesc.contains( "FAT (" ) )
		{
		    ret << "FAT";
		    break;
		}
		if( fullDesc.simplified() == ": data" )
		{
		    ret << "RAW";
		    break;
		}
		ret << tr( "Unknown" );

		//qDebug() << partsMounted.at( i ) << "matches" << mounts.at( j );
		break;
	    }
	}
    }
    //qDebug() << "file: sending results";

    /*foreach( QString part, ret )
    {
	qDebug() << part;
    }*/

    SendPartitionList( ret );

    //clear old errors
    errStr.clear();
}

//triggered once the user entered their password
void UnixFsChecker::PasswordIsEntered()
{
    if( !running )
	return;

    QString pass = rootPass.isEmpty() ? "iLikeDicks" : rootPass; //crashes with an empty password
    process->write( QByteArray( pass.toLatin1() ) );
}

void UnixFsChecker::RunJob( QStringList args )
{
    errStr.clear();
    //stdStr.clear();
    if( running )
    {
	emit SendFatalErr( tr( "file is already running" ), UNIX_FS_CHECKER );
	return;
    }
    QString filePath = "file";

    QString command = runAsRoot ? "sudo" : filePath;
    if( runAsRoot )
    {
	QStringList newArgs = QStringList() << "-S" << filePath;
	foreach( QString arg, args )
	    newArgs << arg;

	args = newArgs;
    }

    running = true;
    process->start( command, args );
    if( !process->waitForStarted() )//default timeout 30,000 msecs
    {
	qDebug() << "failed to start file";
	SendFatalErr( tr( "Error starting file!" ), UNIX_FS_CHECKER );
    }
    //qDebug() << command << args;
    //qDebug() << "file started running ok";

}
void UnixFsChecker::GetFsTypes( QStringList parts )
{
    if( running )
    {
	emit SendFatalErr( tr( "file is already running" ), UNIX_FS_CHECKER );
	return;
    }

    partsResolved.clear();
    partsMounted.clear();

    if( !parts.size() )
    {
	emit SendFatalErr( tr( "Empty partition list @ UNIX FS checker" ), UNIX_FS_CHECKER );
	return;
    }

    partsMounted = parts;

    QSettings settings( settingsPath, QSettings::IniFormat );
    QStringList args = QStringList() << "-s";
    bool argsOk = false;
    QString mtnPath = settings.value( "paths/mountfile" ).toString().isEmpty() ? "/etc/mtab" : settings.value( "paths/mountfile" ).toString();
    QFile file( mtnPath );
    if( file.exists() && file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
	QString mountStr = file.readAll();
	file.close();
	QStringList mounts = mountStr.split( "\n", QString::SkipEmptyParts );
	if( !mounts.isEmpty() )
	{
	    foreach( QString part, parts )
	    {
		int slashes = part.count( "/" );		    //if the path contains 3 or more slashes, remove everything but the first 2 folders
		if( slashes > 2 && part.startsWith( "/media/" ) )   // .../media/someFolder/wbfs becomes /media/someFolder
		{

		    int curSlash = 0;
		    int slashPos = 0;
		    while( curSlash < 2 )
		    {
			slashPos = part.indexOf( "/", slashPos + 1 );
			curSlash++;
		    }
		    part.resize( slashPos );
		}
		bool found = false;
		foreach( QString mp, mounts )
		{
		    if( mp.contains( part ) )
		    {
			//qDebug() << part << "->" << mp;
			mp.resize( mp.indexOf( " " ) );
			partsResolved << mp;
			args << mp;
			found = true;
		    }

		}
		if( !found )
		{
		    args << part;
		    partsResolved << part;
		}
	    }
	    argsOk = true;
	}
    }

    if( !argsOk )//something went wrong looking for the mounts file, just use the partitions we were given
    {
	foreach( QString part, parts )
	{
	    args << part;
	    partsResolved << part;
	}
    }
    RunJob( args );
}

//wait for wit to finish
bool UnixFsChecker::Wait( int msecs )
{
    process->closeReadChannel( QProcess::StandardOutput );
    process->closeReadChannel( QProcess::StandardError );
    process->closeWriteChannel();
    return process->waitForFinished( msecs );
}

//send a kill message
void UnixFsChecker::Kill()
{
    emit KillProcess();
}
