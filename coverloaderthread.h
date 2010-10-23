#ifndef COVERLOADERTHREAD_H
#define COVERLOADERTHREAD_H

#include "includes.h"

class CoverLoaderThread : public QThread
{
    Q_OBJECT

 public:
     CoverLoaderThread( QObject *parent = 0 );
     ~CoverLoaderThread();

     void CheckCovers( const QStringList s, QString baseFolder, QStringList paths, int t, bool l );
     void ForceQuit();

 protected:
     void run();

 signals:
     void SendProgress( int );
     void SendDone( QList< QImage >, int t );
     void CoverIsMissing( QString id, int t );


 //public slots:

 private:
     QMutex mutex;
     QWaitCondition condition;
     QStringList ids;
     QString basePath;
     QStringList subDirs;

     QImage Get( QString id );
     bool Have( QString id );

     QImage To160x224( QImage i );

     int type;

     bool abort;
     bool load;
 };

Q_DECLARE_METATYPE(QList<QImage>)

#endif // COVERLOADERTHREAD_H
