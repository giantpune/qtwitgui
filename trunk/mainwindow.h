#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "includes.h"

#include "wiitdbwindow.h"
#include "custommdiitem.h"
#include "covermanagerwindow.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    int pFlowDirection;
    int pFlowDelay;
    bool alreadyAskingForPassword;

    QMap<QString, QList<QTreeWidgetItem *> > gameMap;//holds partition name, games
    QList<QTreeWidgetItem *> partList;		    //holds partition paths and their settings
    QList<QTreeWidgetItem *> ReadPartitionSettings();

    //keep track of the wiitdb subwindow
    bool wiiTDBisOpen;
    WiiTDBWindow *wiiTDBwindow;
    CustomMdiItem *subWiiTDB;
    void CreateWiiTDBSubWindow();

    //keep track of the coverflow subwindow
    bool pFlowIsOpen;
    CoverManagerWindow *pFlow;
    CustomMdiItem *subPFlow;
    void CreatePFlowSubWindow();

    //functions for detecting which type of mdi window is active
    CustomMdiItem *findMdiChild( const QString &name, int type );
    CustomMdiItem *CurrentMdiSubWindow();
    int CurrentMidSubwindowType();

    void LoadSettings();
    void SaveSettings();

    void DisableEnablePartitionWindows();
    void CheckWit();

private slots:
    void on_actionRefresh_Current_Window_triggered();
    void on_actionSave_As_triggered();
    void on_actionSave_triggered();
    void on_actionTest_triggered();
    void on_actionOpenGame_triggered();
    void on_actionCovers_triggered(bool checked);
    void on_actionWiiTDB_triggered(bool checked);
    void on_actionSettings_triggered();
    void on_actionOpen_Partition_triggered();
    void on_actionAbout_triggered();

    void MdiItemDestroyed( QString name, QPoint pos, QSize size, int type );

    void ReceiveListFor_1_Partition( QString, QList<QTreeWidgetItem *> );
    void RecieveUpdatedPartitionInfo( QTreeWidgetItem * );
    void ReceiveAllPartitionInfo( QList<QTreeWidgetItem *> );
    void OpenSelectedPartitions( QList<QTreeWidgetItem *> list );
    void NeedToAskForPassword();
    void OpenGames( QStringList );
    void ReactToInvalidPartionReport( QString part );

    void on_menuFile_aboutToShow();
    void on_menuView_aboutToShow();



protected:
    void closeEvent( QCloseEvent * closeEvent );
    void dropEvent( QDropEvent *event );
    void dragEnterEvent( QDragEnterEvent *event );

signals:
    void TellOpenWindowsThatTheSettingsAreChanged();
    void SendNewPartitionListToSubWindows( QList<QTreeWidgetItem *> );
    void UserEnteredPassword();
    void SendListsToCoverManager( QMap<QString, QList<QTreeWidgetItem *> > );//send all the current gamelists and partition names to the covermanager
};

#endif // MAINWINDOW_H
