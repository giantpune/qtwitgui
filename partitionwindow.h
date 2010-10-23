#ifndef PARTITIONWINDOW_H
#define PARTITIONWINDOW_H

#include "includes.h"
#include "withandler.h"
#include "wwthandler.h"
#include "gc_shrinkthread.h"

namespace Ui {
    class PartitionWindow;
}

class PartitionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PartitionWindow( QWidget *parent = 0 );
    ~PartitionWindow();

    //set the window title and path of this window
    void SetPartition( QTreeWidgetItem *part );
    void SetPartitionAndGameList( QTreeWidgetItem * part, QList<QTreeWidgetItem *> gameList );

private:
    Ui::PartitionWindow *ui;
    QTreeWidgetItem *partition;
    WitHandler wit;
    WwtHandler wwt;

    bool writingToWBFS;//keep track of a WBFS partition if we are writing to it so we can enable it when done
    QString busyWBFSPath;
    void SetPartitionEnabled( QString part, bool enabled );

    QList<QTreeWidgetItem *> partList;//holds a copy of the partition list

    //stuff for aligning gamecube games
    GC_ShrinkThread *gcGame;
    int gcTotalgames;
    bool shrinkingGc;
    bool gcSingleFile;
    QStringList gcGameList;
    QString gcDestination;


private slots:
    void on_actionRefresh_List_triggered();
    void HandleWiimmsErrors( QString err, int id );
    void GetPartitionInfo( QList<QTreeWidgetItem *> games, QString MibUsed );
    void TreeSelectionChanged( QTreeWidgetItem * current, QTreeWidgetItem * previous );
    void CustomTreeWidgetContentmenu( const QPoint& pos );
    void HideProgressBar( int job );
    void NeedToAskForPassword();
    void GetStatusTextFromWiimms( QString text );
    void ShrinkNextGame();
    //void AskForGameListIfThereIsntOne();

public slots:
    void SettingsHaveChanged();
    void SetPartitionList( QList<QTreeWidgetItem *> pList );
    void GetPasswordFromMainWindow();


protected:
    void closeEvent( QCloseEvent * closeEvent );

signals:
    void SendGamelistFor_1_Partition( QString, QList<QTreeWidgetItem *> );
    void UserEnteredPassword();
    void GameClicked( QString id );
    void SendUpdatedPartitionInfo( QTreeWidgetItem * );
    void AskMainWindowForPassword();
    void BrowseGames( QStringList );
    void ReportInvalidPartition( QString );
};

#endif // PARTITIONWINDOW_H
