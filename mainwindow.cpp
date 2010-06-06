#include <stdio.h>
#include <stdlib.h>
#include <qfiledialog.h>
#include <QTreeWidget>
#include <QMessageBox>
#include <stdarg.h>
#include <QStringList>
#include <qtextstream.h>
#include <unistd.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filefolderdialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow( parent ), ui( new Ui::MainWindow )
{
    ui->setupUi( this );
    ui->plainTextEdit->clear();

    witRunning = 0;
    undoLastTextOperation = false;
    alreadyGotTitle = false;

    //create the pointer to the process used to run wit
    witProcess = new QProcess( this );

    //connect output and input signals between the process and the main window so we can get information from it
    //and also send a "kill" message if the main window is closed while the process is running
    connect(witProcess, SIGNAL( readyReadStandardOutput() ), this, SLOT( ReadyReadStdOutSlot() ) );
    connect(witProcess, SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( ProcessFinishedSlot(  int, QProcess::ExitStatus ) ) );
    connect(this, SIGNAL( KillProcess() ), witProcess, SLOT( kill() ) );

    //get the version of wit and append it to the titlebar
    QString str = "./wit";
    witProcess->start( str );
    if( !witProcess->waitForStarted() )//default timeout 30,000 msecs
    {
	qDebug() << "failed to start wit";
	ui->statusBar->showMessage( "Error starting wit!" );
    }

    //create and add the tree window
    groupIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
    groupIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);
    keyIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

    ui->treeWidget->header()->resizeSection( 0, 300 );


    extractAct = new QAction( "Extract", this );
    replaceAct = new QAction( "Replace", this );

    ui->treeWidget->addAction( extractAct );
    ui->treeWidget->addAction( replaceAct );
    ui->treeWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(extractAct, SIGNAL(triggered()), this, SLOT(ExtractSlot()));
    connect(replaceAct, SIGNAL(triggered()), this, SLOT(ReplaceSlot()));

    ui->statusBar->showMessage( "Ready" );
}

//destructor
MainWindow::~MainWindow()
{
    delete ui;
    if( witRunning )
    {
	emit KillProcess();
//	qsleep( 1 );
    }
    delete( witProcess );
    //if( filepaths )
	//delete( filepaths );
}

//copy text
void MainWindow::on_pushButton_3_clicked()
{
    ui->plainTextEdit->selectAll();
    ui->plainTextEdit->copy();
}

//append text to the message box
void MainWindow::AddText( const char in[] )
{
    ui->plainTextEdit->insertPlainText( in );
}

//input file
void MainWindow::on_toolButton_clicked()
{

    //QFileDialog dialog(this);
    FileFolderDialog dialog(this);
    //dialog.setFileMode(QFileDialog::Directory);
    dialog.setNameFilter( "*.iso *.wbfs *.ciso *.wdf" );
    dialog.setDirectory( ui->lineEdit_default_path->text() );

    if (dialog.exec())
    {
	QString item = dialog.selectedFiles()[ 0 ];
	QByteArray ba  = item.toLatin1();

	ui->lineEdit->setText( item );
	UpdateOptions();
    }
}

//send command to wit
void MainWindow::on_pushButton_4_clicked()
{
    if( witRunning )
    {
	QMessageBox::warning(this, tr("Slow your roll!"),tr("Wit is still running.\nWait for the current job to finish."),"Ok");
	return;
    }
    witRunning = 1;
    ui->progressBar->setValue( 0 );

    QStringList args;
    args << "CP";				// copy command
    args << ui->lineEdit->text();		//source path
    args << ui->lineEdit_2->text();		//dest path

    //region
    QString reg;
    switch( ui->comboBox->currentIndex() )
    {
	case 0:
	default:
	    break;
	case 1:
	    QTextStream( &reg ) << "--region=" << 0;//jap
	    args << reg;
	    break;
	case 2:
	    QTextStream( &reg ) << "--region=" << 1;//usa
	    args << reg;
	    break;
	case 3:
	    QTextStream( &reg ) << "--region=" << 2;//pal
	    args << reg;
	    break;
	case 4:
	    QTextStream( &reg ) << "--region=" << 4;//korea
	    args << reg;
	    break;
    }

    //ios
    if( ui->spinBox->value() != tmdIOS )
    {
	QString ios;
	QTextStream( &ios ) << "--ios=" << ui->spinBox->value();
	args << ios;
    }

    //id
    if( ui->checkBox_7->isChecked() )
    {
	args << "--id=" + ui->lineEdit_3->text();
    }

    //title
    if( ui->checkBox_6->isChecked() )
    {
	args << "\'--name=" + ui->lineEdit_4->text() + "\'";
    }

    //modify
    if( ( ui->checkBox_2->isChecked() ||
	ui->checkBox_3->isChecked() ||
	ui->checkBox_4->isChecked() )
	&& ( ui->checkBox_6->isChecked() ||
	ui->checkBox_7->isChecked() ) )
    {
	QString mod = "--modify=";
	u8 checked = 0;

	if( ui->checkBox_2->isChecked() )
	{
	    checked++;
	    mod += "DISC";
	}
	if( ui->checkBox_3->isChecked() )
	{
	    if( checked )mod += ",";
	    mod += "BOOT";
	    checked++;
	}
	if( ui->checkBox_4->isChecked() )
	{
	    if( checked )mod += ",";
	    mod += "TMD,TICKET";
	}

	args << mod;
    }

    //verbose
    if( ui->verbose_combobox->currentIndex() )
    {
	if( ui->verbose_combobox->currentIndex() == 1)
	    args << "--quiet";
	else
	{
	    for( int i = 1; i < ui->verbose_combobox->currentIndex(); i++ )
	    args << "-v";
	}
    }

    //logging
    for( int i = 0; i < ui->logging_combobox->currentIndex(); i++ )
	args << "-L";

    //overwrite existing files
    if( ui->overwrite_checkbox->isChecked() )
	args << "--overwrite";

    //test mode
    if( ui->checkBox->isChecked() )
	args << "--test";

    //clear the current text window
    ui->plainTextEdit->clear();
    ui->tabWidget->setDisabled( true );

    //make sure we get the progress output
    args << "--progress";

    //show the command in the console window
    QString str = "./wit";
    ui->plainTextEdit->insertPlainText( str + " " );
    for( int i = 0; i < args.size(); i++ )
	ui->plainTextEdit->insertPlainText( args[ i ] + " " );

    //start a process using wit from the current working directory
    witProcess->start( str, args );
    if( !witProcess->waitForStarted() )//default timeout 30,000 msecs
    {
	qDebug( "!waitforstarted()" );
	ui->statusBar->showMessage( "Error starting wit!" );
	return;
    }
    ui->statusBar->showMessage( "Wit is running..." );
}

//output file
void MainWindow::on_toolButton_2_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setDirectory( ui->lineEdit_default_path->text() );
    dialog.setNameFilter( "*.iso *.wbfs *.ciso *.wdf" );

    if (dialog.exec())
    {
	QString item = dialog.selectedFiles()[ 0 ];
	if( !item.isEmpty() )
	    ui->lineEdit_2->setText( item );
    }
}

//write new region string
void MainWindow::UpdateRegStr( int i )
{
    /*region = i;
    switch( i )
    {
	case 1:
	    sprintf(regStr, "JAP" );
	    break;

	case 2:
	    sprintf(regStr, "USA" );
	    break;

	case 3:
	    sprintf(regStr, "PAL" );
	    break;

	case 4:
	    sprintf(regStr, "KOR" );
	    break;

	default:
	    memset( regStr, 0, strlen( regStr ) );
	    break;
    }*/
}

//region changed
void MainWindow::on_comboBox_currentIndexChanged( int index )
{
    //this->UpdateRegStr( index );
}

//update the window & available settings
void MainWindow::UpdateOptions()
{
    // title & ID line edits
    ui->lineEdit_4->setEnabled( ui->checkBox_6->isChecked() );
    ui->lineEdit_3->setEnabled( ui->checkBox_7->isChecked() );

    // check boxes
    bool checked = ui->checkBox_6->isChecked() || ui->checkBox_7->isChecked();
    ui->checkBox_2->setEnabled( checked );
    ui->checkBox_3->setEnabled( checked );
    ui->checkBox_4->setEnabled( checked );

    //title & id
    char path[ 256 ];
    snprintf( path, sizeof( path ), "%s/sys/boot.bin", ui->lineEdit->text().toLatin1().data() );

    FILE *f = fopen( path, "rb" );
    if( !f )
    {
	snprintf( path, sizeof( path ), "%s/DATA/sys/boot.bin", ui->lineEdit->text().toLatin1().data() );
	f = fopen( path, "rb" );
    }
    if( f )
    {
	fseek( f, 0, 0 );
	fread( &id, 6, 1, f );
	ui->lineEdit_3->setText( id );

	fseek( f, 0x20, 0 );
	fread( &name, 0x40, 1, f );
	ui->lineEdit_4->setText( name );

	fclose( f );
    }

    //IOS
    snprintf( path, sizeof( path ), "%s/tmd.bin", ui->lineEdit->text().toLatin1().data() );
    f = fopen( path, "rb" );
    if( !f )
    {
	snprintf( path, sizeof( path ), "%s/DATA/sys/tmd.bin", ui->lineEdit->text().toLatin1().data() );
	f = fopen( path, "rb" );
    }
    if( f )
    {
	fseek( f, 0x18b, 0 );
	fread( &tmdIOS, 1, 1, f );
	fclose( f );

	if( ui->default_ios_spinbox->value() < 3 )
	{
	    ui->spinBox->setValue( tmdIOS );
	}
	else
	{
	    ui->spinBox->setValue( ui->default_ios_spinbox->value() );
	}

	ui->spinBox->setDisabled( false );

	QString m = "\"" + ui->lineEdit->text() + "\" loaded";
	ui->statusBar->showMessage( m );
    }
    else
    {
	ui->spinBox->setDisabled( true );
	QString m = "\"" + ui->lineEdit->text() + "\" is not a valid game directory";
	ui->statusBar->showMessage( m );
    }
}

void MainWindow::on_checkBox_6_clicked()
{
    this->UpdateOptions();
}

void MainWindow::on_checkBox_7_clicked()
{
    this->UpdateOptions();
}

//after typing in the "source" text field
void MainWindow::on_lineEdit_editingFinished()
{
    this->UpdateOptions();
}

//get message from the workthread
void MainWindow::ShowMessage( const QString &s )
{
    ui->plainTextEdit->insertPlainText( s );
    if( s.contains( "%" ) )
	qDebug( "\"%s\" contains %% ", s.toLatin1().data() );
}

//get "done" status from the workthread
void MainWindow::GetThreadDone( int i )
{
    witRunning = i;
    ui->statusBar->showMessage( "Ready for more work" );
}

//get messages from the procces running wit and convert the messages to stuff to use in the GUI
void MainWindow::ReadyReadStdOutSlot()
{
    //read text from wit
    QString read = witProcess->readAllStandardOutput();

    //this is the initial run of wit triggered by creating teh window.  just get the program name & version from it and return;
    if( read.contains( "wit: Wiimms") && !alreadyGotTitle )
    {
	QString s = read;
	s = s.trimmed();
	s.resize( s.indexOf( "\n", 0) - 1 );
	setWindowTitle( s );
	//now merge the stdout and stderr into 1 channel for easier reading while actually doing work
	witProcess->setReadChannelMode( QProcess::MergedChannels );
	alreadyGotTitle = true;
	return;
    }

    //get rid of stupid windows new lines
    read.replace( "\r\n", "\n" );

    //delete the last text appended to the console if the last message was flagged to be deleted
    if( undoLastTextOperation )
		{
		    ui->plainTextEdit->undo();
		    undoLastTextOperation = false;
		}

    //does the current message need to be flagged to be deleted next time this function is called?
    if( read.contains( "\r" ) )
	undoLastTextOperation = true;

    //add the current text
    ui->plainTextEdit->insertPlainText( read );

    if( ui->tabWidget->currentIndex() == 0 )
    {
	//turn the % message into a int and pass it to the progress bar
	 if( read.contains( "%" ) )
	{
	     QString str = read.simplified(); //remove extra whitespace
	     QString numText;
	     int perChar = str.indexOf( "%" );
	     int num = 0;
	     if( perChar < 4 )
	     {
		     for( int i = 0; i < perChar; i++ ) //copy the number to another string
			numText += str.toLatin1().data()[ i ];

		     num = numText.toInt();//convert to int
		     if( num < 101 )
			 ui->progressBar->setValue( num );
		     //qDebug( "numText: %s\nnum: %d", numText.toLatin1().data(), num );
	     }
	}
     }
    else if( ui->tabWidget->currentIndex() == 1 )
    {
	//get all the text output from wit and run it into 1 string
	//read.remove( QChar('\r') );
	filepaths += read;
    }
}
void MainWindow::ProcessFinishedSlot( int i, QProcess::ExitStatus s )
{
//    qDebug( "process is done running\nExitCode: %d\nExitStatus: %d", i, s );
    if( !i && !s )
    {
	ui->progressBar->setValue( 100 );
	ui->plainTextEdit->insertPlainText( "Done!" );
    }
    else
    {
	QString st;
	QTextStream( &st ) <<"Done, but with error [ ExitCode: " << i << "  ErrorStatus: " << s << "]";
	ui->plainTextEdit->insertPlainText( st );
    }

    if( ui->tabWidget->currentIndex() == 1 && alreadyGotTitle )
    {
	ui->statusBar->showMessage( "Got FST list from wit, parsing it into a pretty file tree..." );
	ParseFileList();
    }
    ui->statusBar->showMessage( "Ready" );
    witRunning = 0;
    ui->tabWidget->setDisabled( false );
}

//load game to edit
void MainWindow::on_edit_img_pushButton_clicked()
{
    //currentReadLine = 0;

    //if( filepaths ) delete( filepaths );
    //filepaths = new QStringList;
    QString path = QFileDialog::getOpenFileName( this, tr("Open Wii Image"), "/media/1TB_3GbS_2/BackUp_of_Wii_HDD/wbfs", tr("Wii Games (*.iso *.wbfs *.wdf *.ciso)"));
    if( path.isEmpty() )
	return;

    //clear the last loaded ISO
    filepaths.clear();
    while( ui->treeWidget->takeTopLevelItem( 0 ) );

    QStringList args;
    args << "ILIST-L";
    args << path;

    QString str = "./wit";
    ui->plainTextEdit->clear();
    ui->plainTextEdit->insertPlainText( str + " " );
    ui->tabWidget->setDisabled( true );
    for( int i = 0; i < args.size(); i++ )
	ui->plainTextEdit->insertPlainText( args[ i ] + " " );

    //start a process using wit from the current working directory
    witProcess->start( str, args );
    if( !witProcess->waitForStarted() )//default timeout 30,000 msecs
    {
	qDebug( "!waitforstarted()" );
	ui->statusBar->showMessage( "Error starting wit!" );
	return;
    }
    ui->statusBar->showMessage( "Wit is running..." );
}

//process the long string of fst files to the tree view
void MainWindow::ParseFileList()
{
    //split the output from wit at "\n" and remove spaces and shit
    QStringList list = filepaths.split("\n", QString::SkipEmptyParts);

    //remove the non-file strings
    for ( int i = 0; i < 3 ; ++i )
    {
	list.removeFirst();
    }

    //add each full path to the free view
    for ( int i = 0; i < list.size() ; ++i )
    {
	if( !list[ i ].isEmpty() )
	{
	    AddItemToTree( list[ i ] );
	    //update the progressbar
	    ui->progressBar->setValue( (int)( ( (float)( i + 1 ) / (float)list.size() ) * (float)100 ) );
	}
    }
    ui->progressBar->setValue( 100 );
}

//adds an item to the tree view given a full path
void MainWindow::AddItemToTree( const QString s )
{
    //qDebug() << "AddItemToTree( " << s << " )";
    QString path = s;
    path = path.trimmed();
    QString sizeText;

    //get the size from the start of the string
    while( !path.startsWith( " " ) )
    {
	sizeText += path.at( 0 );
	path.remove( 0, 1 );
    }
    path.remove( 0, 1 );



    int index = 0;

    QTreeWidgetItem *parent = ui->treeWidget->invisibleRootItem();

    // start at the beginning of the full path and add each folder to the tree if it needs to be added
    // change the pointer to the new folder and continue down the string till we get to the end
    while( !path.isEmpty() )
    {
	QString string;
	bool isFolder = false;
	int firstSlash = path.indexOf( "/" );

	if( firstSlash > -1 )
	{
	    for( int i = 0; i < firstSlash; i++ ) //copy the first part of the path to another string
		string += path.toLatin1().data()[ i ];

	    isFolder = true;
	}
	else string = path;

	//qDebug() << path;

	if( findItem( string, parent, index ) == -1 )
	{
	    parent = createItem( string, parent, childCount( parent ) );
	}
	else
	{
	    parent = childAt( parent, findItem( string, parent, index ) );
	}

	if( firstSlash > -1 )
	    path.remove( 0, firstSlash + 1 );

	else
	    path.clear();

	//set the icon for this item
	if( isFolder )
	    parent->setIcon(0, groupIcon);
	else
	{
	    parent->setIcon( 0, keyIcon );
	    parent->setText( 2, sizeText );
	}
    }
}

//returns the index of the child named "s" of the given parent or -1 if the child is not found
int MainWindow::findItem( const QString s, QTreeWidgetItem *parent, int startIndex )
{
//    qDebug() << "findItem( " << s << " )";
    int ret = -1;
    for( int i = startIndex; i < childCount( parent ); i++ )
    {
	if ( childAt( parent, i )->text( 0 ) == s )
	{
	    ret = i;
	    break;
	}
    }
 //   qDebug() << " = " << ret;
    return ret;
}

//returns how many child items an item has ( non-recursive )
int MainWindow::childCount( QTreeWidgetItem *parent )
{
    if ( parent )
	return parent->childCount();
    else
	return ui->treeWidget->topLevelItemCount();
}

//returns a reference to a item at index of given parent
QTreeWidgetItem *MainWindow::childAt( QTreeWidgetItem *parent, int index )
{
    if ( parent )
	return parent->child( index );
    else
	return ui->treeWidget->topLevelItem( index );
}

//add a new item to the tree view as a chald of the given parent
QTreeWidgetItem *MainWindow::createItem(const QString &text, QTreeWidgetItem *parent, int index)
{
    //qDebug() << "adding " << text << " index " << index;
    QTreeWidgetItem *after = 0;
    if (index != 0)
	after = childAt(parent, index - 1);

    QTreeWidgetItem *item;
    if (parent)
	item = new QTreeWidgetItem( parent , after);
    else
	item = new QTreeWidgetItem( ui->treeWidget , after);

    item->setText(0, text);
    //item->setFlags(item->flags() | Qt::ItemIsEditable);
    return item;
}

//this is triggered on right-click -> extract
void MainWindow::ExtractSlot()
{
    extractPaths.clear();
    //qDebug() << "extractSlotTriggered ";
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidget->selectedItems();
    for( int i = 0; i < selectedItems.size(); i++ )
    {
	extractPaths << ItemToFullPath( selectedItems[ i ] );
    }

    /*for( int i = 0; i < extractPaths.size(); i++ )
    {
	qDebug() << extractPaths[ i ] ;
    }*/
}

//this is triggered on right-click -> replace
void MainWindow::ReplaceSlot()
{
    qDebug() << "replaceSlotTriggered";
}

//returns the full path of a item in the tree view
QString MainWindow::ItemToFullPath( QTreeWidgetItem * item )
{
    QString key = item->text(0);
    QTreeWidgetItem *ancestor = item->parent();
    while ( ancestor ) {
	key.prepend(ancestor->text(0) + "/");
	ancestor = ancestor->parent();
    }
    return key;
}
//"clear" button clicked
void MainWindow::on_pushButton_2_clicked()
{
    ui->plainTextEdit->clear();
}
