#ifndef COVERMANAGERWINDOW_H
#define COVERMANAGERWINDOW_H

#include "includes.h"
#include "pictureflow.h"
#include "coverloaderthread.h"

enum
{
    coverType2d = 777,
    coverType3d,	//not supported in the coverflow crap, but used only to get the list of missing covers
    coverTypeFull,
    coverTypeDisc
};

namespace Ui {
    class CoverManagerWindow;
}


class DownloadManager: public QObject
{
    Q_OBJECT
public:
    DownloadManager( QObject *parent = 0, const QString &base = QString(), const QString &loc = QString() );

    void append( const QString &id, int t );
    void append( const QStringList &idList, int t  );
    QString saveFileName( const QString &id, int t );

    void SetLocale( const QString &loc );
    void SetBasePath( const QString &p );

signals:
    void finished();

private slots:
    void startNextDownload();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
    void downloadReadyRead();

private:
    QNetworkAccessManager manager;
    QQueue< QMap< QString, int > > downloadQueue;
    QNetworkReply *currentDownload;
    QFile output;
    QTime downloadTime;

    int downloadedCount;
    int totalCount;

    QString baseCoverPath;
    QString locale;
};

class CoverManagerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CoverManagerWindow(QWidget *parent = 0);
    ~CoverManagerWindow();
    PictureFlow *PFlowObject();
    void ShowSidePane( bool show = true );
    void ShowTextAndProgress( bool show = true );
    //bool Exists( );

private:
    Ui::CoverManagerWindow *ui;
    QMap< QString, QStringList > gameLists;//each entry holds the name of a partition and a list of all it's games in ID6
    QDir coverDir;

    CoverLoaderThread loader;

    QStringList loadedList;
    QMap< int, QStringList > missingCovers;//int is the cover type, the lists are the IDs
    bool canCheckMoreCovers;

    DownloadManager manager;



signals:
    void NewIDInFocus( QString );//send whenever a game is clicked or centered - for wiitdb


public slots:
    void SetGameLists( QMap<QString, QList<QTreeWidgetItem *> > gameMap );
    void LoadCoversForPartition( QString part );
    //void SettingsHaveChanged();
    //void ListMissingCovers( int type );

private slots:

private slots:
    void on_frame_customContextMenuRequested(QPoint pos);
    void ReceiveCovers( QList< QImage >, int t );
    void CoverHasBeenSelected( int );
    void ReceiveMissingCover( QString id, int type );
};




#endif // COVERMANAGERWINDOW_H
