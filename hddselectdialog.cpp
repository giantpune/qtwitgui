#include "ui_hddselectdialog.h"
#include "hddselectdialog.h"
#include "passworddialog.h"

#include "includes.h"
#include "tools.h"
#ifdef Q_WS_WIN
    #include "windowsfsstuff.h"
#endif
//static QList<QTreeWidgetItem *> pList;//remember the items between multiple instances

HDDSelectDialog::HDDSelectDialog( QWidget *parent ) : QDialog( parent ), ui( new Ui::HDDSelectDialog )
{
    ui->setupUi( this );
    ui->treeWidget->header()->resizeSection( 0, 300 );
    ui->treeWidget->setHeaderHidden( false );

    QFontMetrics fm( fontMetrics() );
    ui->treeWidget->header()->resizeSection( 0, fm.width( QString( 17, 'W' ) ) );//path
    ui->treeWidget->header()->resizeSection( 1, fm.width( "WWWWW" ) );//#games
    ui->treeWidget->header()->resizeSection( 2, fm.width( "WWWWWWW" ) );//size
    ui->treeWidget->header()->resizeSection( 3, fm.width( "WWWWWWW" ) );//split
    ui->treeWidget->header()->resizeSection( 4, fm.width( "WWWWW" ) );//source

    oktoRequestNextLIST_LLL = true;
    alreadyAskingForPassword = false;
    setWindowTitle( tr( "Partition Selection" ) );

    connect( ui->treeWidget, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( CustomTreeWidgetContentmenu( const QPoint& ) ) );
#ifndef Q_WS_WIN
    QSettings s( settingsPath, QSettings::IniFormat );
    bool root = s.value( "root/enabled" ).toBool();
    wit.SetRunAsRoot( root );
    wwt.SetRunAsRoot( root );
    unixFs.SetRunAsRoot( root );
    wit.SetNamesFromWiiTDB();


    connect( &wwt, SIGNAL( RequestPassword() ), this, SLOT( NeedToAskForPassword() ) );
    connect( this, SIGNAL( UserEnteredPassword() ), &wwt, SLOT( PasswordIsEntered() ) );

    connect( &wit, SIGNAL( RequestPassword() ), this, SLOT( NeedToAskForPassword() ) );
    connect( this, SIGNAL( UserEnteredPassword() ), &wit, SLOT( PasswordIsEntered() ) );

    connect( &unixFs, SIGNAL( RequestPassword() ), this, SLOT( NeedToAskForPassword() ) );
    connect( this, SIGNAL( UserEnteredPassword() ), &unixFs, SLOT( PasswordIsEntered() ) );


    connect( &unixFs, SIGNAL( SendPartitionList( QStringList ) ), this, SLOT( GetFsTypes( QStringList ) ) );
    connect( &unixFs, SIGNAL( SendFatalErr( QString, int ) ), this, SLOT( HandleWiimmsErrors( QString, int ) ) );
#endif

    connect( &wwt, SIGNAL( SendPartitionList( QStringList ) ), this, SLOT( GetWBFSPartitionList( QStringList ) ) );
    connect( &wwt, SIGNAL( SendFatalErr( QString, int ) ), this, SLOT( HandleWiimmsErrors( QString, int ) ) );
    connect( &wit, SIGNAL( SendFatalErr( QString, int ) ), this, SLOT( HandleWiimmsErrors( QString, int ) ) );
    connect( &wit, SIGNAL( SendListLLL( QList<QTreeWidgetItem *>, QString ) ), this, SLOT( GetPartitionInfo( QList<QTreeWidgetItem *>, QString ) ) );
}

HDDSelectDialog::~HDDSelectDialog()
{
    delete ui;
}

void HDDSelectDialog::DestroyProcessesAndWait()
{
    setCursor( Qt::BusyCursor );
    wit.Kill();
    wwt.Kill();
#ifndef Q_WS_WIN
    unixFs.Kill();
    unixFs.Wait();
#endif
    wit.Wait();
    wwt.Wait();
    unsetCursor();
}

void HDDSelectDialog::closeEvent( QCloseEvent * closeEvent )
{
    DestroyProcessesAndWait();
    QWidget::closeEvent( closeEvent );
}

//add items to the list if no partition with the same name is already there
void HDDSelectDialog::AddPartitionsToList( QList<QTreeWidgetItem *> list )
{
    for( int i = 0; i < list.size(); i++ )
    {
	bool found = false;
	for( int j = 0; j < ui->treeWidget->topLevelItemCount(); j++ )
	{
	    if( list.at( i )->text( 0 ) == ui->treeWidget->topLevelItem( j )->text( 0 ) )
		found = true;
	}
	if( !found )
	{
	    QTreeWidgetItem *item = list.at( i )->clone();
	    if( !item->text( 2 ).contains( "." ) )//this size text doesnt contain a ".", so assume we havent already converted it from bytes to GiB
		item->setText( 2, SizeTextGiB( item->text( 2 ) ) );
	    ui->treeWidget->addTopLevelItem( item );
	}
    }
}

//they clicked "ok" button
void HDDSelectDialog::accept()
{
    DestroyProcessesAndWait();

    QList<QTreeWidgetItem *> selected = ui->treeWidget->selectedItems();
    if( !selected.isEmpty() )
	emit SendSelectedPartition( selected );

    emit SendHDDList( ui->treeWidget->invisibleRootItem()->takeChildren() );
    QDialog::accept();
}

//they clicked "cancel"
void HDDSelectDialog::reject()
{
    DestroyProcessesAndWait();
    emit SendHDDList( ui->treeWidget->invisibleRootItem()->takeChildren() );
    QDialog::reject();
}

#ifndef Q_WS_WIN
//wit/wwt has asked for a password
void HDDSelectDialog::NeedToAskForPassword()
{
    if( alreadyAskingForPassword )
	return;

    alreadyAskingForPassword = true;
    PasswordDialog dialog;
    dialog.exec();
    alreadyAskingForPassword = false;
    emit UserEnteredPassword();
}
#endif
//manually add partition / folder
void HDDSelectDialog::on_pushButton_manualADD_clicked()
{
    QString str = QFileDialog::getExistingDirectory( this, tr( "Select a folder" ) );
    if( str.isEmpty() )
	return;

    AddNewPartitionToList( str, tr( "Manual" ) );
}

//auto add partition / folders
void HDDSelectDialog::on_pushButton_find_clicked()
{
    ui->buttonBox->setEnabled( false );
    oktoRequestNextLIST_LLL = true;
    QList<QTreeWidgetItem *> lst = ui->treeWidget->invisibleRootItem()->takeChildren();
    for( int j = 0; j < lst.size(); j++ )
    {
	QTreeWidgetItem *it = lst.at( j );
	delete it;
    }
#ifdef Q_WS_WIN
    QFileInfoList list = QDir::drives();

#else
    QDir dir( "/media" );

    dir.setFilter( QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::NoSymLinks );
    QFileInfoList list = dir.entryInfoList();

#endif
    for( int i = 0; i < list.size(); ++i )
    {
        QFileInfo fileInfo = list.at( i );
        QDir subDir( fileInfo.absoluteFilePath() );
        //QDir subDir( dir.absoluteFilePath( fileInfo.fileName() ) );

	if( subDir.exists( "wbfs" ) )
	    AddNewPartitionToList( subDir.absoluteFilePath( "wbfs" ), tr( "Auto" ) );

	if( subDir.exists( "iso" ) )
	    AddNewPartitionToList( subDir.absoluteFilePath( "iso" ), tr( "Auto" ) );

	if( subDir.exists( "games" ) )
	    AddNewPartitionToList( subDir.absoluteFilePath( "games" ), tr( "Auto" ) );
    }
    wwt.GetPartitions();
}

//receive WBFS partitions from wwt
void HDDSelectDialog::GetWBFSPartitionList( QStringList list )
{
    unsetCursor();
    foreach( QString part, list )
    {
	AddNewPartitionToList( part, "wwt" );
	//wit.ListLLL_HDD( part );
    }
}

//respond somehow to fatal errors
void HDDSelectDialog::HandleWiimmsErrors( QString err, int id )
{
    qDebug() << "wiimms error" << err << id;
    unsetCursor();
    ui->buttonBox->setEnabled( true );//probably shouldnt enable both of these right here every time.
    ui->pushButton_reScan->setEnabled( true );
    switch( id )
    {
	case witListLLLHDD:
	{
	    QTreeWidgetItem * item = PartitionBeingRead();
	    if( item )
	    {
		item->setText( 1, "0" );
		item->setText( 2, tr( "Wit Error" ) + ": " + err );
		break;
	    }

	    break;
	}

    }
}

//which partition is currently being read
QTreeWidgetItem *HDDSelectDialog::PartitionBeingRead()
{
    for( int i = 0; i < ui->treeWidget->topLevelItemCount(); i++ )
    {
	QTreeWidgetItem * item = ui->treeWidget->topLevelItem( i );
	if( item->text( 1 ) == tr( "Reading" ) )
	{
	    return item;
	}
    }
    qDebug() << "error @ partition being read";
    return 0;
}

//check a path and if it doesn't mach anything already on our list, add it.  trigger the "LIST-LLL" chain to start
void HDDSelectDialog::AddNewPartitionToList( QString path, QString source )
{
    QList<QTreeWidgetItem *> found = ui->treeWidget->findItems( path, Qt::MatchFixedString );

    if( !found.isEmpty() )
    {
	qDebug() << "partition already exists" << path;
	return;
    }

    QTreeWidgetItem * item = new QTreeWidgetItem( QStringList() << path );
    item->setText( 4, source );
    if( source == "wwt" )
        item->setText( 5, "WBFS" );
    ui->treeWidget->addTopLevelItem( item );

    //read partition settings
    QSettings settings( settingsPath, QSettings::IniFormat );
    int size = settings.beginReadArray( "partitionOptions" );
    for( int i = 0; i < size; i++ )
    {
	 settings.setArrayIndex( i );
	 if( settings.value( "path" ).toString() == item->text( 0 ) )
	     item->setText( 3, settings.value( "split" ).toString() );
    }
    settings.endArray();

    RequestNextLIST_LLLL();
}

//receive info about a specific partition from wit ( response to list-lll )
void HDDSelectDialog::GetPartitionInfo( QList<QTreeWidgetItem *> games, QString MibUsed )
{
    //qDebug() << MibUsed;
    QTreeWidgetItem *item = PartitionBeingRead();
    if( !item )
    {
	qDebug() << "!PartitionBeingRead()";
	return;
    }
    oktoRequestNextLIST_LLL = true;
    RequestNextLIST_LLLL();
    QString count;
    QTextStream( &count ) << games.size();
    item->setText( 1, count );
    item->setText( 2, SizeTextGiB( MibUsed ) );
#ifdef Q_WS_WIN
    QString fs = WindowsFsStuff::GetFilesystem( item->text( 0 ) );//this lookup is blocking, so it will freeze the gui while it quieries the drive.
    if( !fs.isEmpty() )
    {
        if( fs.contains( "FAT", Qt::CaseInsensitive ) )
        {
            if( item->text( 0 ).endsWith( "/games" ) )//FAT partition ending with a folder called "games"  flag it as SNEEK
            {
                item->setText( 5, "SNEEK" );
            }
            else					//no "games" folder, set the flag to split large files
            {
                item->setText( 5, fs );
                item->setText( 3, tr( "Yes" ) );
            }
        }
    }
#endif

    if( item == ui->treeWidget->topLevelItem( ui->treeWidget->topLevelItemCount() - 1 ) )
    {
	ui->pushButton_reScan->setEnabled( true );
#ifdef Q_WS_WIN
        ui->buttonBox->setEnabled( true );
        unsetCursor();
#endif
    }

    emit SendGamelistFor_1_Partition( item->text( 0 ), games );
}

//look for the next HDD in the list that hasn't been scanned and ask the data from wit
void HDDSelectDialog::RequestNextLIST_LLLL()
{
    ui->pushButton_reScan->setEnabled( false );
    if( !oktoRequestNextLIST_LLL )//only list-lll 1 hdd at a time
    {
	qDebug() << "!oktoRequestNextLIST_LLL";
	return;
    }

    for( int i = 0; i < ui->treeWidget->topLevelItemCount(); i++ )
    {
	QTreeWidgetItem * item = ui->treeWidget->topLevelItem( i );
	if( item->text( 1 ).isEmpty() )
	{
	    item->setText( 1, tr( "Reading" ) );
	    oktoRequestNextLIST_LLL = false;
	    setCursor( Qt::BusyCursor );
	    wit.ListLLL_HDD( item->text( 0 ) );
	    return;
	}
    }
    // all HDDs have been, or are being scanned
    //ui->buttonBox->setEnabled( true );
    //unsetCursor();
#ifndef Q_WS_WIN
    RequestFsTypes();
#endif
}

//clear the gamecounts & Mib used and rescan them
void HDDSelectDialog::on_pushButton_reScan_clicked()
{
    ui->buttonBox->setEnabled( false );
    if( !oktoRequestNextLIST_LLL )//dont allow this button to be clicked until this dialog has "settled"
	return;

    ui->pushButton_reScan->setEnabled( false );

    for( int i = 0; i < ui->treeWidget->topLevelItemCount(); i++ )
    {
	QTreeWidgetItem * item = ui->treeWidget->topLevelItem( i );
	item->setText( 1, QString() );
	item->setText( 2, QString() );
    }

    oktoRequestNextLIST_LLL = true;
    RequestNextLIST_LLLL();
}

//context menu for the partition tree window
void HDDSelectDialog::CustomTreeWidgetContentmenu( const QPoint& pos )
{
    //qDebug() << "HDDSelectDialog::CustomTreeWidgetContentmenu" << pos;
    QPoint globalPos = ui->treeWidget->viewport()->mapToGlobal(pos);
    QTreeWidgetItem* selected = ui->treeWidget->itemAt( pos );
    if( !selected )
    {
	//no item is at the spot clicked
	return;
    }

    //bool checked = ui->treeWidget->selectedItems().at( 0 )->text( 3 ) == tr( "Yes" );
    bool checked = selected->text( 3 ) == tr( "Yes" );

    QMenu myMenu;

    QAction changeSplitAct( tr( "Split Games When Writing to This Partition"), &myMenu );
    changeSplitAct.setCheckable( true );
    changeSplitAct.setChecked( checked );

    QMenu fsMenu( tr( "Filesystem"), &myMenu );
    QAction wbfsFsAct( "WBFS", &fsMenu );
    QAction fatFsAct( "FAT", &fsMenu );
    QAction sneekFsAct( "SNEEK", &fsMenu );
    QAction ntfsFsAct( "NTFS", &fsMenu );
    QAction extFsAct( "EXT", &fsMenu );
    QAction hpfsFsAct( "HPFS", &fsMenu );

    myMenu.addAction( &changeSplitAct );
    fsMenu.addAction( &wbfsFsAct );
    fsMenu.addAction( &fatFsAct );
    fsMenu.addAction( &sneekFsAct );
    fsMenu.addAction( &ntfsFsAct );
    fsMenu.addAction( &extFsAct );
    fsMenu.addAction( &hpfsFsAct );

    myMenu.addMenu( &fsMenu );

    QAction* selectedAct = myMenu.exec( globalPos );
    if( selectedAct )
    {
	// something was chosen, do stuff
	if( selectedAct == &changeSplitAct )//split games
	{
	    foreach( QTreeWidgetItem *item, ui->treeWidget->selectedItems() )
	    {
		item->setText( 3, checked ? QString() : tr( "Yes" ) );
	    }
	}
	else if( selectedAct == &wbfsFsAct || selectedAct == &fatFsAct || selectedAct == &ntfsFsAct //change FS type
		 || selectedAct == &extFsAct || selectedAct == &hpfsFsAct )
	{
	    foreach( QTreeWidgetItem *item, ui->treeWidget->selectedItems() )
	    {
		item->setText( 5, selectedAct->text() );
	    }
	}
    }
    else
    {
	// nothing was chosen
    }
}

//run a process with "file..." to get the different filesystems
void HDDSelectDialog::RequestFsTypes()
{
    int size = ui->treeWidget->topLevelItemCount();
    QStringList list;
    for( int i = 0; i < size; i++ )
	list << ui->treeWidget->topLevelItem( i )->text( 0 );
#ifndef Q_WS_WIN
    unixFs.GetFsTypes( list );
#endif
    //qDebug() << "unixFs.GetFsTypes" << list;
}

//receive the list of filetypes from "file"
void HDDSelectDialog::GetFsTypes( QStringList list )
{
    //qDebug() << "HDDSelectDialog::GetFsTypes";
    //qDebug() << list;
    int size = list.size();
    if( size % 2 )
    {
	qDebug() << "HDDSelectDialog::GetFsTypes invlid list size";
	ui->buttonBox->setEnabled( true );
	unsetCursor();
	return;
    }
    int size2 = ui->treeWidget->topLevelItemCount();
    for( int i = 0; i < size; i += 2 )
    {
	for( int j = 0; j < size2; j++ )
	{
	    QTreeWidgetItem *item = ui->treeWidget->topLevelItem( j );
	    if( item->text( 0 ) == list.at( i ) )
	    {
		item->setText( 5, list.at( i + 1 ) == "RAW" ? "WBFS" : list.at( i + 1 ) );
		if( list.at( i + 1 ).contains( "FAT" ) )
		{
		    if( item->text( 0 ).endsWith( "/games" ) )//FAT partition ending with a folder called "games"  flag it as SNEEK
			item->setText( 5, "SNEEK" );
		    else					//no "games" folder, set the flag to split large files
			item->setText( 3, tr( "Yes" ) );
		}
		break;
	    }
	}
    }
    ui->buttonBox->setEnabled( true );
    unsetCursor();
}

