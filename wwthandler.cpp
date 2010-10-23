#include "includes.h"
#include "wwthandler.h"
#include "tools.h"

extern QString rootPass;
extern QString rootAskStr;
extern QString rootWrongStr;
extern QString rootFailStr;

WwtHandler::WwtHandler( QObject *parent, bool root ) :QObject( parent )
{
    runAsRoot = root;
    //requestedPassword = false;
    wwtJob = wwtNoJob;
    //create the pointer to the process used to run wit
    process = new QProcess( this );

#ifdef Q_WS_WIN
    // Add an environment variable to shut up the cygwin warning in windows
    //QStringList env = QProcess::systemEnvironment();//depreciated
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("CYGWIN", "nodosfilewarning");
    process->setProcessEnvironment( env );
#endif

    //connect output and input signals between the process and the main window so we can get information from it
    //and also send a "kill" message if this object is destroyed while the process is running
    connect( process, SIGNAL( readyReadStandardOutput() ), this, SLOT( ReadyReadStdOutSlot() ) );
    connect( process, SIGNAL( readyReadStandardError() ), this, SLOT( ReadyReadStdErrSlot() ) );
    connect( process, SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( ProcessFinishedSlot(  int, QProcess::ExitStatus ) ) );
    connect( this, SIGNAL( KillProcess() ), process, SLOT( kill() ) );
}

WwtHandler::~WwtHandler()
{
    if( wwtJob != wwtNoJob )
    {
	emit KillProcess();
    }
    delete process;
    process = 0;
}

void WwtHandler::SetRunAsRoot( bool root )
{
    runAsRoot = root;
}

void WwtHandler::ReadyReadStdOutSlot()
{
    //read text from wwt
    QString curRead = process->readAllStandardOutput();
#ifdef Q_WS_WIN
    //get rid of stupid windows new lines
    curRead.replace( "\r\n", "\n" );
#endif
    stdStr += curRead;

    //qDebug() << "gotmessage out" << curRead;

    switch ( wwtJob )
    {
	case wwtAdd:
	    if( curRead.contains( " - ADD " ) )
	    {
		currJobText = curRead;
		currJobText.remove( 0, curRead.indexOf( " - ADD " ) + 7 );
		currJobText.resize( currJobText.indexOf( " " ) );
		emit SendMessageForStatusBar( currJobText );
		break;
	    }
	    //turn the % message into a int and pass it to the progress bar
	    if( curRead.contains( "%" ) )
	    {
		 QString str = curRead.simplified(); //remove extra whitespace
		 QString numText;
		 int perChar = str.indexOf( "%" );
		 int num = 0;
		 if( perChar < 4 && perChar > 0)
		 {
		     while( str.at( 0 ) != '%' )
		     {
			 numText += str.at( 0 );
			 str.remove( 0, 1 );
		     }

		    num = numText.toInt();//convert to int
		    if( num < 101 )
			//ui->progressBar->setValue( num );
			emit SendProgress( num );
		 }

		 num = str.indexOf( "ETA", 0 );
		 if( num > 1 )
		 {
		     str.remove( 0, num );
		     emit SendMessageForStatusBar( currJobText + " : " + str );
		     //qDebug() << "wwtAdd:" << str;
		     //ui->statusBar->showMessage( tr( "Wit is running..." ) + " " + str );
		 }
		 break;
	    }
	    //qDebug() << "unhandled wwtAdd text:" << curRead;
	    break;
	default:
	    break;
    }


}

void WwtHandler::ReadyReadStdErrSlot()
{
    //read text from wwt
    errStr += process->readAllStandardError();

    qDebug() << "gotmessage err" << errStr;
#ifdef Q_WS_WIN
    //get rid of stupid windows new lines
    errStr.replace( "\r\n", "\n" );
#endif
    if( errStr.startsWith( rootWrongStr ) )
    {
	errStr.clear();
	rootPass.clear();
    }

    if( errStr.startsWith( rootAskStr ) )
    {
	errStr.clear();
	//requestedPassword = true;
	emit RequestPassword();
	return;
	/*while( alreadyAskingForPassword ) sleep( 1 ); //there is already a password dialog, wait for the user to answer it
	if( rootPass.isEmpty() )
	{
	    alreadyAskingForPassword = true;
	    PasswordDialog dialog;
	    dialog.exec();
	    alreadyAskingForPassword = false;
	}
	QString pass = rootPass.isEmpty() ? "iLikeDicks" : rootPass; //crashes with an empty password
	process->write( QByteArray( pass.toLatin1() ) );
	return;*/
    }
    if( errStr == rootFailStr )
    {
	emit KillProcess();
	process->waitForFinished();//force kill the thing
	qDebug() << "killed sudo";
	emit SendFatalErr( errStr, wwtJob );
	errStr.clear();
	rootPass.clear();
	wwtJob = wwtNoJob;
    }
}

void WwtHandler::ProcessFinishedSlot( int i, QProcess::ExitStatus s )
{
    qDebug() << "wwt finished:" << i << s << "job:" << wwtJob;
    if( i || s )
    {
#ifdef Q_WS_WIN
	if( i == 128 )
	{
	    emit SendFatalErr( tr( "Maybe cygwin1.dll is missing" ), wwtJob );
	    errStr.clear();
	    wwtJob = wwtNoJob;
	    return;
	}
#endif
	if( s )
	{
	    emit SendFatalErr( tr( "Wwt appears to have crashed" ), wwtJob );
	    errStr.clear();
	    wwtJob = wwtNoJob;
	    return;
	}

	if( !errStr.isEmpty() )
	{
	    emit SendFatalErr( errStr, wwtJob );
	    errStr.clear();
	    wwtJob = wwtNoJob;
	    return;
	}

	if( i == 4 )
	{
	    emit SendFatalErr( tr( "At least 1 operation was ignored.<br>Maybe you didn't set the \"overwrite\" option?" ), wwtJob );
	    errStr.clear();
	    wwtJob = wwtNoJob;
	    return;
	}
    }

    SendProgress( 100 );
    switch( wwtJob )
    {
	case wwtFind:
	{
	    QStringList parts = stdStr.split( "\n", QString::SkipEmptyParts );
	    emit SendPartitionList( parts );
	}
	case wwtAdd:
	case wwtRemove:
	{
	    emit SendJobDone( wwtJob );
	}
	break;

	default:
	break;





    }

    //clear old errors
    errStr.clear();
    wwtJob = wwtNoJob;
}

//triggered once the user entered their password
void WwtHandler::PasswordIsEntered()
{
    //qDebug() << "wwt: password entered" << requestedPassword;
    if( wwtJob == wwtNoJob )
	return;

    //requestedPassword = false;
    QString pass = rootPass.isEmpty() ? "iLikeDicks" : rootPass; //crashes with an empty password
    process->write( QByteArray( pass.toLatin1() ) );
}

void WwtHandler::RunJob( QStringList args, int jobType )
{
    errStr.clear();
    stdStr.clear();
    if( wwtJob != wwtNoJob )
    {
	emit SendFatalErr( tr( "Wwt is already running" ), jobType );
	return;
    }
    QString wwtPath = GetWwtPath();
    if( wwtPath.isEmpty() )
    {
	emit SendFatalErr( tr( "Job aborted."), jobType );
	return;
    }
    emit SendProgress( 0 );

    QString command = runAsRoot ? "sudo" : wwtPath;
    if( runAsRoot )
    {
	QStringList newArgs = QStringList() << "-S" << wwtPath;
	foreach( QString arg, args )
	    newArgs << arg;

	args = newArgs;
    }

    wwtJob = jobType;
    process->start( command, args );
    if( !process->waitForStarted() )//default timeout 30,000 msecs
    {
	qDebug() << "failed to start wwt";
	SendFatalErr( tr( "Error starting wwt!" ), jobType );
    }

}
void WwtHandler::GetPartitions()
{
    QStringList args = QStringList() << "FIND";
    RunJob( args, wwtFind );
}

//get the wwt path from the settings and check that it exists
QString WwtHandler::GetWwtPath()
{
    QSettings settings( settingsPath, QSettings::IniFormat );
    QString ret = settings.value( "paths/wwt" ).toString();
    if( ret.isEmpty() )
    {
	emit SendFatalErr( tr("No path is set for wwt.  Look in the settings."), wwtJob );
    }
    if( !QFile::exists( ret ) )
    {
	emit SendFatalErr( tr("Invalid path is set for wwt.  Look in the settings."), wwtJob );
    }

    return ret;
}

//wait for wit to finish
bool WwtHandler::Wait( int msecs )
{
    process->closeReadChannel( QProcess::StandardOutput );
    process->closeReadChannel( QProcess::StandardError );
    process->closeWriteChannel();
    return process->waitForFinished( msecs );
}
//send a kill message
void WwtHandler::Kill()
{
    emit KillProcess();
}
