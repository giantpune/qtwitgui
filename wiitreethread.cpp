#include <stdio.h>
#include <stdlib.h>
#include <QProcess>
#include <QtDebug>
#include <QFont>

#include "wiitreethread.h"



WiiTreeThread::WiiTreeThread( QObject *parent ) : QThread( parent )
{
    abort = false;
}

WiiTreeThread::~WiiTreeThread()
{
//    qDebug( "Thread destruct" );
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

//sets the variables and then starts the thread.  this is a 1 time run thread.
//! s is a long QString af paths to turn into a tree.  separated by \n
//! b is weather or not to skip entries with "/.svn" in them
//! ficon is the folder icon to use
//! dicon is the icon to use for directories
void WiiTreeThread::DoCommand( const QString s, bool b, QIcon ficon, QIcon dicon, const QStringList list )
{
    QMutexLocker locker(&mutex);
    inCommand = s;
    skipSvn = b;
    groupIcon = dicon;
    keyIcon = ficon;
    encryptedOffsets = list;
    if ( !isRunning() )
    {
	//even with a game using 13,000+ files, theres is less than a second difference in lowest and time-critical priority.
	//it just happens too fast
	start( NormalPriority );
	//start( QThread::TimeCriticalPriority );

    }
    else //not used since this is a 1 time use thread.  just leaving it here for reference material
    {
	condition.wakeOne();
	//QString str = "Thread already running\n";
	//emit SendText( str );
    }
}

void WiiTreeThread::run()
{
//    forever {
//	qDebug( "Thread run" );

	if ( abort )
	{
	    qDebug( "Thread abort" );
	    return;
	}
	QString str;
	mutex.lock();
	returnItem = new QTreeWidgetItem;
	if( inCommand.isEmpty() )
	{
	    qDebug() << "its empty";
	    return;
	}

	//split the output from wit at "\n" and remove spaces and shit
	QStringList list = inCommand.split("\n", QString::SkipEmptyParts );
	mutex.unlock();

	//remove the non-file strings
	for ( int i = 0; i < 3 ; i++ )
	{
	    //qDebug() << "removing:" << list.at( 0 );
	    list.removeFirst();
	}

	//add each full path to the free view
	for ( int i = 0; i < list.size(); i++ )
	{
	    if ( abort )
	    {
		qDebug( "Thread abort called" );
		return;
	    }
	    if( skipSvn && list[ i ].contains( "/.svn/" ) ) continue;
	    if( !list[ i ].isEmpty() )
	    {
		AddItemToTree( list[ i ] );
		emit SendProgress( (int)( ( (float)( i + 1 ) / (float)list.size() ) * (float)100 ) );
	    }
	}
	emit SendProgress( 100 );
	emit SendDone( returnItem );


	//not used since this is a 1 time run thread
	//leaving it here for reference
	    /*mutex.lock();
	    //make the thread wait for another command
	    condition.wait( &mutex );
	    mutex.unlock();
   }*/

}

//adds an item to the tree view given a full path
void WiiTreeThread::AddItemToTree( const QString s )
{
    QString path = s;
    path = path.simplified();
    QString sizeText;
    QString offsetText = "0x";
    QFont monoFont( "Courier", 8, QFont::Bold );
    bool isSysFolder = s.endsWith( "sys/" ) && s.count( "/" ) == 2;

    //get the offset from the start of the string
    while( !path.startsWith( " " ) )
    {
	offsetText += path.at( 0 );
	path.remove( 0, 1 );
    }
    path.remove( 0, 1 );

    //get the size from the start of the string
    while( !path.startsWith( " " ) )
    {
	sizeText += path.at( 0 );
	path.remove( 0, 1 );
    }
    path.remove( 0, 1 );

    if( sizeText.startsWith( "N=" ) )
    {
	sizeText.remove( 0, 2 );
	sizeText = tr( "%1 files" ).arg( sizeText );
    }

    int index = 0;

    QTreeWidgetItem *parent = returnItem;

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
	    parent->setText( 1, offsetText );
	}
    }


    parent->setText( 2, sizeText );
    parent->setFont( 1, monoFont );
    parent->setFont( 2, monoFont );
    parent->setTextAlignment( 1, Qt::AlignRight | Qt::AlignVCenter );
    if( isSysFolder )
    {
	parent->setText( 1, encryptedOffsets.at( 0 ) );
	encryptedOffsets.removeAt( 0 );
    }

}

//returns the index of the child named "s" of the given parent or -1 if the child is not found
int WiiTreeThread::findItem( const QString s, QTreeWidgetItem *parent, int startIndex )
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
int WiiTreeThread::childCount( QTreeWidgetItem *parent )
{
//    if ( parent )
	return parent->childCount();
//    else
//	return ui->treeWidget->topLevelItemCount();
}

//returns a reference to a item at index of given parent
QTreeWidgetItem *WiiTreeThread::childAt( QTreeWidgetItem *parent, int index )
{
//    if ( parent )
	return parent->child( index );
//    else
//	return ui->treeWidget->topLevelItem( index );
}

//add a new item to the tree view as a chald of the given parent
QTreeWidgetItem *WiiTreeThread::createItem( const QString &text, QTreeWidgetItem *parent, int index )
{
    //qDebug() << "adding " << text << " index " << index;
    QTreeWidgetItem *after = 0;
    if ( index != 0 )
	after = childAt( parent, index - 1 );

    QTreeWidgetItem *item;
//    if (parent)
	item = new QTreeWidgetItem( parent , after );
//    else
//	item = new QTreeWidgetItem( ui->treeWidget , after);

    item->setText( 0, text );
    //item->setFlags(item->flags() | Qt::ItemIsEditable);
    return item;
}



