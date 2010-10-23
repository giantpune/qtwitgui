#ifndef WIITREETHREAD_H
#define WIITREETHREAD_H
// about 95% of this class is pieced together from the Qt examples, 2% I wrote on purpose, and 3% I accidentally wrote and it worked so it stayed


#include "includes.h"


class WiiTreeThread : public QThread
 {
    Q_OBJECT

 public:
     WiiTreeThread( QObject *parent = 0 );
     ~WiiTreeThread();

     void DoCommand( const QStringList s, bool b, QIcon ficon, QIcon dicon, int type, const QStringList list );
     void ForceQuit();

 protected:
     void run();

 signals:
     void SendText( const QString & ); // not used here, but left in for copy/paste thread creating later
     void SendProgress( int );
     void SendDone( QTreeWidgetItem * );


 //public slots:

 private:
     QMutex mutex;
     QWaitCondition condition;
     QStringList inCommand;
     QTreeWidgetItem *returnItem;

     QIcon groupIcon;
     QIcon keyIcon;
     QFont nameFont;
     QFont monoFont;
     QStringList encryptedOffsets;

     bool abort;
     bool skipSvn;
     int gameType;
     int findItem(  const QString s, QTreeWidgetItem *parent );
     //int childCount( QTreeWidgetItem * parent );
     //QTreeWidgetItem *childAt( QTreeWidgetItem *parent, int index );
     //QTreeWidgetItem *createItem( const QString &text, QTreeWidgetItem *parent, int index );
     void AddItemToTree( const QString s, QTreeWidgetItem *parent );
 };

#endif // WIITREETHREAD_H
