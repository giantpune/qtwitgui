/************************************************************************************
*
*   - QtWitGui -				2010 giantpune
*
*   the multilingual, multiplatform, multiformat gui for messing with
*   Wii game images.
*
*   This software comes to you with a GPLv3 license.
*   http://www.gnu.org/licenses/gpl-3.0.html
*
*   Basically you are free to modify this code, distribute it, use it in
*   other projects, or anything along those lines.  Just make sure that any
*   derivative work gets the same license.  And don't remove this notice from
*   the derivative work.
*
*   And please, don't be a douche.  If you borrow code from here, don't claim
*   you wrote it.  Share your source code with others.  Even if you are
*   charging a fee for the binaries, let others read the code as somebody has
*   done for you.
*
*************************************************************************************/

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "includes.h"
#include "withandler.h"
#include "wwthandler.h"
#include "wiitreethread.h"

namespace Ui {
    class GameWindow;
}



class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0, QString game = QString(), QList<QTreeWidgetItem *> pList = QList<QTreeWidgetItem *>() );
    ~GameWindow();

    QStringList AvailableActions();//return a list of actions that are allowed to be performed on this game ( save, save as, etc )

private:
    Ui::GameWindow *ui;
    WitHandler wit;
    WwtHandler wwt;
    QString tmpPath;
    bool busy;

    QIcon groupIcon;
    QIcon keyIcon;
    QString color1;
    QString color2;
    QString color3;

    QString lastPathLoadedCorrectly;
    QString gameTypeStr;
    int gameType;//keep track of the stuff we get from wit, so if it is changed, we will know to pass extra options to wit on copying/editing
    int gameIos;
    int gameRegion;
    QString oldName;
    QString oldId;

    QList<QTreeWidgetItem *> sortedList;
    QStringList extractPaths;
    QStringList replacePaths;
    QStringList partitionOffsets;
    WiiTreeThread *wiithread;
    void EnableDisableStuff();

    QList<QTreeWidgetItem *> partList;//list of available partitions to write to
    QStringList GetPatchArgs();
    QString GetChangeList();

    bool writingToWBFS;//keep track of a WBFS partition if we are writing to it so we can enable it when done
    QString busyWBFSPath;
    void SetPartitionEnabled( QString part, bool enabled );

    QString dirtyPartition;//remember which partition we are writing to. on successful write, tell the main window that the gamelist needs to be reloaded

    void ClearTreeView();
    void InsertText( QString s, QString c);


private slots:
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_checkBox_id_clicked();
    void on_checkBox_title_clicked();
    void LoadGame( QString path );
    void ReceiveGameInfo( QString type, QString id, QString name, int ios, int region, QStringList files, QStringList partitionOffsets, bool fakesigned );
    void HandleThreadErrors( QString err, int id );
    void ThreadIsDoneRunning( QTreeWidgetItem *i );
    void NeedToAskForPassword();
    void HideProgressBar( int job );
    void GetStatusTextFromWiimms( QString text );
    void ReloadGame();

public slots:
    void GetPasswordFromMainWindow();
    void SettingsHaveChanged();
    void SetPartitionList( QList<QTreeWidgetItem *> pList );

signals:
    void UserEnteredPassword();
    void AskMainWindowForPassword();
    void SendUpdatedPartitionInfo( QTreeWidgetItem * );
    void PartitionIsDirty( QString );

protected:
//    void dropEvent( QDropEvent *event );
//    void dragEnterEvent( QDragEnterEvent *event );
};



#endif // GAMEWINDOW_H
