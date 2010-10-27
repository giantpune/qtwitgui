#include "covermanagerwindow.h"
#include "ui_covermanagerwindow.h"
#include "tools.h"



CoverManagerWindow::CoverManagerWindow(QWidget *parent) : QWidget(parent), ui(new Ui::CoverManagerWindow)
{
    ui->setupUi(this);
    ShowTextAndProgress( true );
    ui->progressBar_loading->setVisible( false );
    canCheckMoreCovers = true;

    QString localeStr = QLocale::system().name();
    localeStr.resize( 2 );
    localeStr = localeStr.toUpper();
    manager.SetLocale( localeStr );
    manager.SetBasePath( "./covers" );

    //pmi = new QGraphicsPixmapItem( QPixmap( ":/covers/disc/R6BP78.png" ) );
    //pmi->setTransformOriginPoint( 80, 80 );

    connect( &loader, SIGNAL( SendProgress( int ) ), ui->progressBar_loading, SLOT( setValue( int ) ) );
    connect( &loader, SIGNAL( SendDone( QList< QImage >, int ) ), this, SLOT( ReceiveCovers( QList< QImage >, int ) ) );
    connect( &loader, SIGNAL( CoverIsMissing( QString, int ) ), this, SLOT( ReceiveMissingCover( QString, int ) ) );

    connect( ui->pictureFlow, SIGNAL( centerIndexChanged( int ) ), this, SLOT( CoverHasBeenSelected( int ) ) );
    connect( ui->pictureFlow, SIGNAL( ImageClicked( int ) ), this, SLOT( CoverHasBeenSelected( int ) ) );

}

CoverManagerWindow::~CoverManagerWindow()
{
    delete ui;
}

//expose the pictureflow widget so the main window can directly connect signals and shit to it
PictureFlow *CoverManagerWindow::PFlowObject()
{
    return ui->pictureFlow;
}

void CoverManagerWindow::ShowSidePane( bool show )
{
    ui->groupBox_sideBox->setVisible( show );
}

void CoverManagerWindow::ShowTextAndProgress( bool show )
{
    ui->groupBox_sideBox->setVisible( show );
    ui->label_info->setVisible( show );
    ui->progressBar_download->setVisible( show );

    resize( width(), minimumSizeHint().height() );
}

void CoverManagerWindow::SetGameLists( QMap<QString, QList<QTreeWidgetItem *> > gameMap )
{
    qDebug() << "CoverManagerWindow::SetGameLists";
    gameLists.clear();
    QMap<QString, QList<QTreeWidgetItem *> >::iterator i = gameMap.begin();
    while( i != gameMap.constEnd() )
    {
	QStringList ids;
	QString partName = i.key();
	int size = i.value().size();
	for( int j = 0; j < size; j++ )
	{
	    ids << gameID( i.value().at( j ) );
	}

	if( size )
	    gameLists.insert( partName, ids );

	i++;
    }
    //debug shit
    QMap<QString, QStringList >::iterator p = gameLists.begin();
    while( p != gameLists.constEnd() )
    {
	qDebug() << p.key() << p.value().size();
	/*int size = p.value().size();
	for( int j = 0; j < size; j++ )
	{
	    qDebug() << p.value().at( j );
	}*/

	p++;
    }

}

void CoverManagerWindow::on_frame_customContextMenuRequested(QPoint pos)
{
    //qDebug() << "CoverManagerWindow::on_frame_customContextMenuRequested" << pos;
    QPoint globalPos = ui->frame->mapToGlobal( pos );
    //create the context menu based on the games selected
    QMenu myMenu( this );

    //submenu for download options
    QMenu dlM( tr( "Download Missing Images" ), &myMenu );
    QAction cvrCheck( tr( "Check for missing images"), &dlM );
    QAction cvrFlat( tr( "Front %1").arg( "160x224" ), &dlM );
    QAction cvrFull( tr( "Full %1").arg( "1024x680" ), &dlM );
    QAction cvr3d( tr( "3-D %1").arg( "176x248" ), &dlM );
    QAction cvrDvd( tr( "Disc %1").arg( "160x160" ), &dlM );
    dlM.addAction( &cvrCheck );
    if( canCheckMoreCovers )
    {
	dlM.addAction( &cvrFlat );
	dlM.addAction( &cvrFull );
	dlM.addAction( &cvr3d );
	dlM.addAction( &cvrDvd );
    }
    myMenu.addMenu( &dlM );

    QMenu partMenu( tr( "Sync With Partition" ), &myMenu );
    QList<QAction*> partActions;
    QMap<QString, QStringList >::iterator p = gameLists.begin();
    while( p != gameLists.constEnd() )
    {
	QAction* newAct = new QAction( p.key(), &partMenu );
	partActions << newAct;
	p++;
    }
    if( partActions.size() )
    {
	partMenu.addActions( partActions );
	myMenu.addMenu( &partMenu );
    }

    myMenu.addSeparator();

    //execute the menu
    QAction* selectedItem = myMenu.exec( globalPos );
    //respond to what was selected
    if( selectedItem )
    {
	// something was chosen, do stuff
	int size = partActions.size();
	for( int i = 0; i < size; i++ )
	{
	    if( selectedItem != partActions.at( i ) )
		continue;

	    LoadCoversForPartition( selectedItem->text() );
	    break;
	}
    }
}

void CoverManagerWindow::LoadCoversForPartition( QString part )
{
    QMap<QString, QStringList >::iterator p = gameLists.find( part );
    if( p == gameLists.end() )
    {
	qDebug() << "CoverManagerWindow::LoadCoversForPartition : part not found" << part;
	return;
    }

    loadedList.clear();
    int size = p.value().size();
    if( !size )
    {
	qDebug() << "CoverManagerWindow::LoadCoversForPartition : no games" << part;
	return;
    }
    //dont add duplicates to the coverflow.
    //with several thousand games, this will slow down a lot, but for a partition with 500 games, it happens in < 1 second for me
    for( int j = 0; j < size; j++ )
    {
	if( !loadedList.contains( p.value().at( j ) ) )
	    loadedList << p.value().at( j );
    }

    ui->progressBar_loading->setValue( 0 );
    ui->progressBar_loading->setVisible( true );
    loader.CheckCovers( loadedList, "./covers", QStringList() << "full", coverType2d, true );
}

void CoverManagerWindow::ReceiveCovers( QList< QImage > covers, int t )
{
    qDebug() << "CoverManagerWindow::ReceiveCovers";
    ui->progressBar_loading->setVisible( false );
    ui->pictureFlow->clear();
    //QTimer::singleShot( 0, ui->pictureFlow, SLOT( clear() ) );

    while( ui->pictureFlow->slideCount() );//wait for covers to clear
    int size = covers.size();
    for( int i = 0; i < size; i++ )
    {
	ui->pictureFlow->addSlide( covers.at( i ) );
    }
    ui->pictureFlow->setCenterIndex( 0 );

}

//convert a number emited from the pictureflow widget to an ID for the WiiTDB widget
void CoverManagerWindow::CoverHasBeenSelected( int i )
{
    if( i < 0 || i >= loadedList.size() )
	return;

    emit NewIDInFocus( loadedList.at( i ) );
}

void CoverManagerWindow::ReceiveMissingCover( QString id, int type )
{
    qDebug() << "CoverManagerWindow::ReceiveMissingCover" << type << id;
    //manager.append( id, type );
    QMap< int, QStringList >::iterator p = missingCovers.find( type );
    if( p == missingCovers.end() )//no list for this type yet, just add one
    {
	missingCovers.insert( type, QStringList() << id );
	return;
    }

    if( !p.value().contains( id ) )
	p.value() << id;
}



DownloadManager::DownloadManager( QObject *parent, const QString &base, const QString &loc)
    : QObject(parent), downloadedCount(0), totalCount(0), baseCoverPath( base ), locale( loc )
{
}

void DownloadManager::SetLocale( const QString &loc )
{
    locale = loc;
}

void DownloadManager::SetBasePath( const QString &p )
{
    baseCoverPath = p;
}

void DownloadManager::append( const QStringList &idList, int t )
{
    foreach( QString id, idList )
	append( id, t );

    if (downloadQueue.isEmpty())
	QTimer::singleShot(0, this, SIGNAL(finished()));
}

void DownloadManager::append( const QString &url, int t )
{
    if( downloadQueue.isEmpty() )
	QTimer::singleShot( 0, this, SLOT( startNextDownload()));

    QMap< QString, int > dl;
    dl.insert( url, t );
    downloadQueue.enqueue( dl );
    ++totalCount;
}

QString DownloadManager::saveFileName( const QString &id, int t )
{
    QDir dir( baseCoverPath );
    QString subDirPath;
    switch( t )
    {
    case coverType2d:
	subDirPath = "2d";
	break;
    case coverType3d:
	subDirPath = "3d";
	break;
    case coverTypeFull:
	subDirPath = "full";
	break;
    case coverTypeDisc:
	subDirPath = "disc";
	break;
    default:
	return QString();
	break;
    }

    QDir subDir( dir.filePath( subDirPath ) );
    if( !subDir.exists() )
    {
	if( !subDir.mkpath( subDir.absolutePath() ) )
	{
	    qDebug() << "cant create" << subDir.absolutePath();
	    return QString();
	}
    }
    QString ret = subDir.absolutePath() + "/" + id + ".png";
    if( QFile::exists( ret ) )
    {
	if( !QFile::remove( ret ) )
	{
	    qDebug() << "error deleting" << ret;
	    return QString();
	}
    }
    return ret;
}

void DownloadManager::startNextDownload()
{
    qDebug() << "downloading shit";
    if( downloadQueue.isEmpty() )
    {
	qDebug() <<  downloadedCount << "of" << totalCount << "files downloaded successfully";
	emit finished();
	return;
    }

    QMap< QString, int > p = downloadQueue.dequeue();
    QMap< QString, int >::iterator dl = p.begin();

    //qDebug() << dl;
    int type = dl.value();
    QString id = dl.key();
    QString dlUrl;
    QString filename = saveFileName( id, type );
    qDebug() << id << type;
    if( filename.isEmpty() )
    {
	qDebug() << "empty filename" << id << type;
	startNextDownload();
	return;
    }
    output.setFileName( filename );
    if( !output.open( QIODevice::WriteOnly ) )
    {
	qDebug() << "cant open" << filename;
	startNextDownload();
	return;
    }
    switch( type )
	{
	case coverType2d:
	    dlUrl = "http://wiitdb.com/wiitdb/artwork/cover/";
	    break;
	case coverType3d:
	    dlUrl = "http://wiitdb.com/wiitdb/artwork/cover3D/";
	    break;
	case coverTypeFull:
	    dlUrl = "http://wiitdb.com/wiitdb/artwork/coverfull/";
	    break;
	case coverTypeDisc:
	    dlUrl = "http://wiitdb.com/wiitdb/artwork/disc/";
	    break;
	default:
	    startNextDownload();
	    break;
	}
    QChar regCode = id.at( 3 );
    if( regCode == 'E' )
	dlUrl += "US/";
    else if( regCode == 'J' )
	dlUrl += "JA/";
    else if( regCode == 'W' )
	dlUrl += "ZH/";
    else if( regCode == 'K' )
	dlUrl += "KO/";
    else
	dlUrl += locale + "/";

    dlUrl += id + ".png";
    qDebug() << "trying to DL" << dlUrl;

    QUrl url( dlUrl );

    QNetworkRequest request( url );
    currentDownload = manager.get( request );
    connect( currentDownload, SIGNAL( downloadProgress( qint64, qint64 ) ), this, SLOT( downloadProgress( qint64, qint64 ) ) );
    connect( currentDownload, SIGNAL( finished() ), this, SLOT( downloadFinished() ) );
    connect( currentDownload, SIGNAL( readyRead() ), this, SLOT( downloadReadyRead() ) );

    // prepare the output
    //printf("Downloading %s...\n", url.toEncoded().constData());
    downloadTime.start();
}

void DownloadManager::downloadProgress( qint64 bytesReceived, qint64 bytesTotal )
{
    // calculate the download speed
    double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
    QString unit;
    if (speed < 1024) {
	unit = "bytes/sec";
    } else if (speed < 1024*1024) {
	speed /= 1024;
	unit = "kB/s";
    } else {
	speed /= 1024*1024;
	unit = "MB/s";
    }

    qDebug() << QString::fromLatin1( "%1 %2" ).arg( speed, 3, 'f', 1 ).arg( unit );
}


void DownloadManager::downloadFinished()
{
    output.close();

    if( currentDownload->error() )
    {
	output.remove();
	qDebug() << "currentDownload->error()";
    }
    else
    {
	downloadedCount++;
    }

    currentDownload->deleteLater();
    startNextDownload();
}

void DownloadManager::downloadReadyRead()
{
    output.write( currentDownload->readAll() );
}
