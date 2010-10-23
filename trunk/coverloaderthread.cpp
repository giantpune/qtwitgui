#include "coverloaderthread.h"

CoverLoaderThread::CoverLoaderThread(QObject *parent) : QThread(parent)
{
    abort = false;
    qRegisterMetaType<QList<QImage> >();
}

void CoverLoaderThread::ForceQuit()
{
    mutex.lock();
    abort = true;
    mutex.unlock();
}

CoverLoaderThread::~CoverLoaderThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();
    wait();
}
void CoverLoaderThread::CheckCovers( const QStringList s, QString baseFolder, QStringList paths, int t, bool l )
{
    basePath = baseFolder;
    subDirs = paths;
    type = t;
    load = l;
    ids = s;

    if ( !isRunning() )
    {
	start( NormalPriority );

    }
    else //not used since this is a 1 time use thread.  just leaving it here for reference material
    {
	condition.wakeOne();
	//QString str = "Thread already running\n";
	//emit SendText( str );
    }
}

void CoverLoaderThread::run()
{
	if ( abort )
	{
	    qDebug( "Thread abort" );
	    return;
	}
	mutex.lock();
	if( !ids.size() )
	{
	    qDebug() << "its empty";
	    return;
	}
	mutex.unlock();

	//mutex.unlock();

	QList< QImage > returns;


	int size = ids.size();
	for( int i = 0; i < size; i++ )
	{
	    //qDebug() << "id:" << ids.at( i );
	    returns << Get( ids.at( i ) );
	    emit SendProgress( (int)( ( (float)( i + 1 ) / (float)size ) * (float)100 ) );

	}

	//qDebug() << "thread is done";
	emit SendProgress( 100 );
	emit SendDone( returns, type );

}

QImage CoverLoaderThread::Get( QString id )
{
    QImage ret;
    QDir dir( basePath );
    if( !dir.exists() )
    {
	qDebug() << dir << "doesnt exist";
	return ret;
    }
    int s = subDirs.size();
    if( !s )
    {
	qDebug() << "no subdirectories to try";
	return ret;
    }
    for( int i = 0; i < s; i++ )
    {
	QDir subDir( dir.filePath( subDirs.at( i ) ) );
	if( !ret.load( subDir.absolutePath() + "/" + id + ".png" ) )
	    continue;

	ret = To160x224( ret );
	if( ret != QImage() )
	    return ret;
    }

    emit CoverIsMissing( id, type );//cover isnt found, tell whoever is listening so it can be downloaded or added to some list
    if( ret.load( dir.absolutePath() + "/nocover.png" ) )//user defined no-cover
    {
	return To160x224( ret );
    }
    ret.load( ":/images/nocover.png" );//no-cover compiled in this program
    return ret;

}

bool CoverLoaderThread::Have( QString id )
{
    return true;
}

QImage CoverLoaderThread::To160x224( QImage i )
{
    if( i.size() == QSize( 160, 224 ) )//flat 2d image
	return i;

    if( i.size() == QSize( 1024, 680 ) )//full cover ( wiiflow style )
    {
	int fStart = 535;
	QImage front = i.copy( fStart, 0, 1024 - fStart, 680 );
	return front.scaledToHeight( 224 );
    }

    //TODO: i have no clue how to impliment this.  it should be possible with qtransform
    /*if( i.size() == QSize( 176, 248 ) )//3d covers		16,7   162,15  231,7   162, 216
    {

    }*/

    return QImage();
}
