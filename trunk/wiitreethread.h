#ifndef WIITREETHREAD_H
#define WIITREETHREAD_H
// about 95% of this class is pieced together from the Qt examples, 2% I wrote on purpose, and 3% I accidentally wrote and it worked so it stayed


#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTreeWidget>


class WiiTreeThread : public QThread
 {
    Q_OBJECT

 public:
     WiiTreeThread( QObject *parent = 0 );
     ~WiiTreeThread();

     void DoCommand( const QString s, bool b, QIcon ficon, QIcon dicon );

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
     QString inCommand;
     QTreeWidgetItem *returnItem;
     QIcon groupIcon;
     QIcon keyIcon;
     bool abort;
     bool skipSvn;
     int findItem(  const QString s, QTreeWidgetItem *parent, int startIndex );
     int childCount( QTreeWidgetItem * parent );
     QTreeWidgetItem *childAt( QTreeWidgetItem *parent, int index );
     QTreeWidgetItem *createItem( const QString &text, QTreeWidgetItem *parent, int index );
     void AddItemToTree( const QString s );
 };

#endif // WIITREETHREAD_H
