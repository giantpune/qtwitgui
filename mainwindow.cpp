#include <stdio.h>
#include <stdlib.h>
#include <qfiledialog.h>
#include <QTreeWidget>
#include <QMessageBox>
#include <QFontMetrics>
#include <stdarg.h>
#include <QStringList>
#include <qtextstream.h>
#include <unistd.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filefolderdialog.h"

#define SAVEFILENAME "QtWitGui.ini"

//this syntax apparently works for all platforms
#define WIT "./wit"


#define SAFEDELETE( x ) if( x )delete( x )
#define MAX( x, y ) ( ( x ) > ( y ) ? ( x ) : ( y ) )
#define MIN( x, y ) ( ( x ) < ( y ) ? ( x ) : ( y ) )


MainWindow::MainWindow(QWidget *parent) : QMainWindow( parent ), ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    undoLastTextOperation = false;
    //alreadyGotTitle = false;

    ui->plainTextEdit->clear();

    //create the pointer to the process used to run wit
    witProcess = new QProcess( this );

    //connect output and input signals between the process and the main window so we can get information from it
    //and also send a "kill" message if the main window is closed while the process is running
    connect( witProcess, SIGNAL( readyReadStandardOutput() ), this, SLOT( ReadyReadStdOutSlot() ) );
    connect( witProcess, SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( ProcessFinishedSlot(  int, QProcess::ExitStatus ) ) );
    connect( this, SIGNAL( KillProcess() ), witProcess, SLOT( kill() ) );

    //get the version of wit and append it to the titlebar
    QString str = WIT;
    witJob = witGetVersion;
    witProcess->start( str );
    if( !witProcess->waitForStarted() )//default timeout 30,000 msecs
    {
	qDebug() << "failed to start wit";
	ui->statusBar->showMessage( tr( "Error starting wit!" ) );
    }

    //create and add the tree window
	//icons
    groupIcon.addPixmap( style()->standardPixmap( QStyle::SP_DirClosedIcon ), QIcon::Normal, QIcon::Off );
    groupIcon.addPixmap( style()->standardPixmap( QStyle::SP_DirOpenIcon ), QIcon::Normal, QIcon::On );
    keyIcon.addPixmap( style()->standardPixmap( QStyle::SP_FileIcon ) );

	//make sure the name column is wide enough
    ui->treeWidget->header()->resizeSection( 0, 300 );

	//create the actions and stuff for the context menu
    extractAct = new QAction( tr( "Extract" ), this );
    replaceAct = new QAction( tr( "Replace" ), this );
    ui->treeWidget->addAction( extractAct );
    ui->treeWidget->addAction( replaceAct );
    ui->treeWidget->setContextMenuPolicy( Qt::ActionsContextMenu );
    connect( extractAct, SIGNAL( triggered() ), this, SLOT( ExtractSlot() ) );
    connect( replaceAct, SIGNAL( triggered() ), this, SLOT( ReplaceSlot() ) );

    //load settings
    LoadSettings();

    //make sure buttons are wide enough for text
    ResizeGuiToLanguage();

    //everything should be ready to go now
    ui->statusBar->showMessage( tr( "Ready" ) );
}

//destructor
MainWindow::~MainWindow()
{
    //SaveSettings();
    delete ui;
    if( witJob != witNoJob )
    {
	emit KillProcess();
//	qsleep( 1 );
    }
    SAFEDELETE( extractAct  );
    SAFEDELETE( replaceAct );
    SAFEDELETE( witProcess );
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
    FileFolderDialog dialog(this);
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

//send command to wit [tab 1]
void MainWindow::on_pushButton_4_clicked()
{
    if( witJob != witNoJob )
    {
	QMessageBox::warning(this, tr( "Slow your roll!" ),tr( "Wit is still running.\nWait for the current job to finish." ), tr( "Ok" ) );
	return;
    }
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
    QString str = WIT;
    ui->plainTextEdit->insertPlainText( str + " " );
    for( int i = 0; i < args.size(); i++ )
	ui->plainTextEdit->insertPlainText( args[ i ] + " " );
    ui->plainTextEdit->insertPlainText( "\n" );

    //start a process using wit and give it the arg string
    witJob = witCopy;
    witProcess->start( str, args );
    if( !witProcess->waitForStarted() )
    {
	qDebug( "!waitforstarted()" );
	ui->statusBar->showMessage( tr(  "Error starting wit!" ) );
	witJob = witNoJob;
	return;
    }
    ui->statusBar->showMessage( tr( "Wit is running..." ) );
}

//search for output file
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
	ui->spinBox->setDisabled( false );
	tmdIOS = ui->spinBox->value();
	QString m = "Unknown IOS for \"" + ui->lineEdit->text() + "\" - action not supported yet.";
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
}

//get "done" status from the workthread [depreciated]
void MainWindow::GetThreadDone( int i )
{
    //witRunning = i;
    ui->statusBar->showMessage( tr( "Ready for more work" ) );
}

//get messages from the procces running wit and convert the messages to stuff to use in the GUI
void MainWindow::ReadyReadStdOutSlot()
{
    //read text from wit
    QString read = witProcess->readAllStandardOutput();

    //qDebug() << "gotmessage" << read;

    //this is the initial run of wit triggered by creating the window.  just get the program name & version from it and return;
    if( read.contains( "wit: Wiimms") && witJob == witGetVersion )
    {
	QString s = read;
	s = s.trimmed();
	s.resize( s.indexOf( "\n", 0) - 1 );
	//add the svn version of this program
	s += " | Gui: ";
	s += SVNVER;
	setWindowTitle( s );
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

    if( read.contains( "\r" ) )
    {
	//add the current text 1 line at a time so it can be undone if needed
	QString insertText;
	QString readCopy = read;
	while( readCopy.contains( "\n") )
	{
	    while( !readCopy.startsWith( "\n" ) && !readCopy.isEmpty() )
	    {
		   insertText += readCopy.at( 0 );
		   readCopy.remove( 0, 1 );
	    }
	    insertText += "\n";
	    readCopy.remove( 0, 1 );
	    ui->plainTextEdit->insertPlainText( readCopy );
	}
	ui->plainTextEdit->insertPlainText( readCopy );

	//does the current message need to be flagged to be deleted next time this function is called?
	undoLastTextOperation = true;
    }


    else
	ui->plainTextEdit->insertPlainText( read );

    switch ( witJob )
    {
	case witCopy:
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
	    break;

	case witIlist:
	    //get all the text output from wit and run it into 1 string
	    filepaths += read;
	    break;

	case witDump:
	    filepaths += read;
	    break;

	default:
	    break;

    }

}

//triggered after the wit process is done
void MainWindow::ProcessFinishedSlot( int i, QProcess::ExitStatus s )
{
//    qDebug( "process is done running\nExitCode: %d\nExitStatus: %d", i, s );
    if( !i && !s )
    {
	ui->progressBar->setValue( 100 );
	if( witJob != witGetVersion )
	    ui->plainTextEdit->insertPlainText( tr( "Done!" ) );
    }
    else
    {
	QString st;
	QTextStream( &st ) <<"Done, but with error [ ExitCode: " << i << "  ErrorStatus: " << s << "]";
	ui->plainTextEdit->insertPlainText( st );
    }
    QStringList list;

    //qDebug() << "witJob: " << witJob;
    switch ( witJob )
    {
	case witIlist:
	    ui->statusBar->showMessage( tr( "Got FST list from wit, parsing it into a pretty file tree..." ) );
	    ParseFileList();
	    witJob = witNoJob;
	    break;

	case witDump:
	    if( filepaths.isEmpty() )
		break;
	    //split the output from wit at "\n" and remove spaces and shit
	    list = filepaths.split("\n", QString::SkipEmptyParts );
	    foreach( QString str, list )
	    {
		str = str.trimmed();
		if( str.contains( "ID & type" ) )
		{
		    str.remove( 0, 10 );
		    str = str.trimmed();
		    str.resize( 6 );
		    ui->label_edit_id->setText( str );
		}
		else if( str.contains( "Disc name:" ) )
		{
		    str.remove( 0, 10 );
		    str = str.trimmed();
		    ui->label_edit_name->setText( str );
		}
		else if( str.contains( "Region:" ) )
		{
		    str.remove( 0, 7 );
		    str = str.trimmed();
		    ui->label_edit_region->setText( str );
		}

	    }
	    DoIlist();
	    break;


	case witGetVersion:
	    //now merge the stdout and stderr into 1 channel for easier reading while actually doing work
	    witProcess->setReadChannelMode( QProcess::MergedChannels );
	    witJob = witNoJob;
	    break;

	default:
	    witJob = witNoJob;
	    break;

    }

    ui->statusBar->showMessage( tr( "Ready" ) );
    ui->tabWidget->setDisabled( false );


}

//build the ILIST-L command and start the process with it
void MainWindow::DoIlist()
{
//    qDebug() << "DoIlist()";
    if( isoPath.isEmpty() )
    {
	qDebug() << "isoPath.isEmpty()";
	return;
    }

    //clear the last loaded ISO
    filepaths.clear();
    while( ui->treeWidget->takeTopLevelItem( 0 ) );

    QStringList args;
    args << "ILIST-L";
    args << isoPath;

    QString str = WIT;
    ui->plainTextEdit->insertPlainText( "\n" + str + " " );
    ui->tabWidget->setDisabled( true );
    for( int i = 0; i < args.size(); i++ )
	ui->plainTextEdit->insertPlainText( args[ i ] + " " );

    //start a process using wit from the current working directory
    witJob = witIlist;
//    qDebug() << "Starting Ilist - witJob: " << witJob;
    witProcess->start( str, args );
    if( !witProcess->waitForStarted() )
    {
	qDebug( "!waitforstarted()" );
	ui->statusBar->showMessage( tr( "Error starting wit!" ) );
	witJob = witNoJob;
	return;
    }
    ui->statusBar->showMessage( tr( "Wit is running..." ) );
//    qDebug() << "mmmmkay";

}

//load game to edit
void MainWindow::on_edit_img_pushButton_clicked()
{
    FileFolderDialog dialog(this);
    dialog.setNameFilter( "*.iso *.wbfs *.ciso *.wdf" );
    dialog.setDirectory( ui->lineEdit_default_path->text() );

    if ( !dialog.exec() )
	return;

    isoPath = dialog.selectedFiles()[ 0 ];

    if( isoPath.isEmpty() )
	return;

    QStringList args;
    args << "DUMP";
    args << isoPath;

    QString str = WIT;
    ui->plainTextEdit->clear();
    ui->plainTextEdit->insertPlainText( str + " " );
    ui->tabWidget->setDisabled( true );
    for( int i = 0; i < args.size(); i++ )
	ui->plainTextEdit->insertPlainText( args[ i ] + " " );

    //start a process using wit from the current working directory
    witJob = witDump;
    witProcess->start( str, args );
    if( !witProcess->waitForStarted() )
    {
	qDebug( "!waitforstarted()" );
	ui->statusBar->showMessage( tr( "Error starting wit!" ) );
	witJob = witNoJob;
	return;
    }
    ui->statusBar->showMessage( tr( "Wit is running..." ) );
}

//process the long string of fst files to the tree view
void MainWindow::ParseFileList()
{
//    qDebug() << "ParseFileList()";
    if( filepaths.isEmpty() )return;
    //split the output from wit at "\n" and remove spaces and shit
    QStringList list = filepaths.split("\n", QString::SkipEmptyParts );

    //remove the non-file strings
    for ( int i = 0; i < 3 ; i++ )
    {
	list.removeFirst();
    }

    //add each full path to the free view
    for ( int i = 0; i < list.size(); i++ )
    {
	if( ui->checkBox_hiddenFiles->isChecked() &&
	    list[ i ].contains( "/.svn/" ) ) continue;
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

	while( !path.startsWith( "/" ) && !path.isEmpty() )
	{
	    string += path.at( 0 );
	    path.remove( 0, 1 );
	}
	if( path.startsWith( "/" ) )
	{
	    isFolder = true;
	    path.remove( 0, 1 );
	}

	//qDebug() << path;

	if( findItem( string, parent, index ) == -1 )
	{
	    parent = createItem( string, parent, childCount( parent ) );
	}
	else
	{
	    parent = childAt( parent, findItem( string, parent, index ) );
	}

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

//save settings to disc
bool MainWindow::SaveSettings()
{
    QFile file( SAVEFILENAME );
    if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
	 return false;

    QTextStream out( &file );
    out << "test:"	    << ui->checkBox->checkState()
	<< "\noverwrite:"   << ui->overwrite_checkbox->checkState()
	<< "\ntext:"	    << ui->verbose_combobox->currentIndex()
	<< "\nlogging:"	    << ui->logging_combobox->currentIndex()
	<< "\nios:"	    << ui->default_ios_spinbox->value()
	<< "\npath:"	    << ui->lineEdit_default_path->text()
	<< "\nregion:"	    << ui->comboBox->currentIndex()
	<< "\nstarttab:"    << ui->startupTab_combobox->currentIndex()
	<< "\nupdatetitle:" << ui->checkBox_6->checkState()
	<< "\nupdateid:"    << ui->checkBox_7->checkState()
	<< "\ndischdr:"	    << ui->checkBox_2->checkState()
	<< "\ntmdticket:"   << ui->checkBox_4->checkState()
	<< "\nparthdr:"	    << ui->checkBox_3->checkState()
	<< "\ninputpath:"   << ui->lineEdit->text()
	<< "\noutputpath:"  << ui->lineEdit_2->text()
	<< "\nignoreidden:"  << ui->checkBox_hiddenFiles->checkState();

    return true;

}

//read settings file and set values from it
bool MainWindow::LoadSettings()
{
    QFile file( SAVEFILENAME );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
	 return false;

    while ( !file.atEnd() )
    {

	 QString setting;
	 QString value = file.readLine().trimmed();		//get a line from the file and remove whitespace from the start and end
	 bool ok = false;

	 if( value.startsWith( "#" ) || value.isEmpty() )	//allow for # used as comments and empty lines in the ini file just in case
	     continue;

	 //split the string into setting and value
	 while( !value.startsWith( ":" ) )
	 {
		setting += value.at( 0 );
		value.remove( 0, 1 );
	 }
	 value.remove( 0, 1 );

	//match the setting string and then set a value in the gui
	if( setting == "test" )
	{
	    int v = value.toInt( &ok, 10 );			//for checkboxes, 0 is not checked / 2 is checked
	    ui->checkBox->setChecked( ok && v );		//turn "ok" signals that the string was successfully turned into a int ( base 10 )
	}							//use ok & int as a bool value
	else if( setting == "overwrite" )
	{
	    int v = value.toInt( &ok, 10 );
	    ui->overwrite_checkbox->setChecked( ok && v );
	}
	else if( setting == "text" )
	{
	    int v = value.toInt( &ok, 10 );
	    if( ok )
		ui->verbose_combobox->setCurrentIndex( v );
	}
	else if( setting == "logging" )
	{
	    int v = value.toInt( &ok, 10 );
	    if( ok )
		ui->logging_combobox->setCurrentIndex( v );
	}
	else if( setting == "ios" )
	{
	    int v = value.toInt( &ok, 10 );
	    if( ok )
		ui->default_ios_spinbox->setValue( v );
	}
	else if( setting == "region" )
	{
	    int v = value.toInt( &ok, 10 );
	    if( ok )
		ui->comboBox->setCurrentIndex( v );
	}
	else if( setting == "path" )
	{
	    ui->lineEdit_default_path->setText( value );
	}
	else if( setting == "starttab" )
	{
	    int v = value.toInt( &ok, 10 );
	    if( ok )
	    {
		ui->tabWidget->setCurrentIndex( v );
		ui->startupTab_combobox->setCurrentIndex( v );
	    }
	}
	else if( setting == "updatetitle" )
	{
	    int v = value.toInt( &ok, 10 );
	    ui->checkBox_6->setChecked( ok && v );
	}
	else if( setting == "updateid" )
	{
	    int v = value.toInt( &ok, 10 );
	    ui->checkBox_7->setChecked( ok && v );
	}
	else if( setting == "dischdr" )
	{
	    int v = value.toInt( &ok, 10 );
	    ui->checkBox_2->setChecked( ok && v );
	}
	else if( setting == "tmdticket" )
	{
	    int v = value.toInt( &ok, 10 );
	    ui->checkBox_4->setChecked( ok && v );
	}
	else if( setting == "parthdr" )
	{
	    int v = value.toInt( &ok, 10 );
	    ui->checkBox_3->setChecked( ok && v );
	}
	else if( setting == "inputpath" )
	{
	    ui->lineEdit->setText( value );
	}
	else if( setting == "outputpath" )
	{
	    ui->lineEdit_2->setText( value );
	}
	else if( setting == "ignoreidden" )
	{
	    int v = value.toInt( &ok, 10 );
	    ui->checkBox_hiddenFiles->setChecked( ok && v );
	}



    }

    return true;
}

//save button clicked
void MainWindow::on_save_pushButton_clicked()
{
    SaveSettings();
    ui->statusBar->showMessage( tr( "Settings Saved" ), 5000 );
    //qDebug() << "settings saved";
}


void MainWindow::ResizeGuiToLanguage()
{
    int pad = 20;
    int size = 0;
    QFontMetrics fm( fontMetrics() );
    ui->edit_img_pushButton->setMinimumWidth( MAX( ui->edit_img_pushButton->minimumWidth(), fm.width( ui->edit_img_pushButton->text() ) + pad ) );
    ui->pushButton_2->setMinimumWidth( MAX( ui->pushButton_2->minimumWidth(), fm.width( ui->pushButton_2->text() ) + pad ) );
    ui->pushButton_3->setMinimumWidth( MAX( ui->pushButton_3->minimumWidth(), fm.width( ui->pushButton_3->text() ) + pad ) );
    ui->pushButton_4->setMinimumWidth( MAX( ui->pushButton_4->minimumWidth(), fm.width( ui->pushButton_4->text() ) + pad ) );
    ui->save_pushButton->setMinimumWidth( MAX( ui->save_pushButton->minimumWidth(), fm.width( ui->save_pushButton->text() ) + pad ) );
    ui->toolButton->setMinimumWidth( MAX( ui->toolButton->minimumWidth(), fm.width( ui->toolButton->text() ) + pad ) );
    ui->toolButton_2->setMinimumWidth( MAX( ui->toolButton_2->minimumWidth(), fm.width( ui->toolButton_2->text() ) + pad ) );


    for( int i = 0; i < ui->verbose_combobox->count(); i++ )
    {
	size = MAX( size, fm.width( ui->verbose_combobox->itemText( i ) ) + pad );
    }
    ui->verbose_combobox->setMinimumWidth( size );

    for( int i = 0; i < ui->startupTab_combobox->count(); i++ )
    {
	size = MAX( size, fm.width( ui->startupTab_combobox->itemText( i ) ) + pad );
    }
    ui->startupTab_combobox->setMinimumWidth( size );




}
