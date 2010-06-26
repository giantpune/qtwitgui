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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QMutex>
#include <QProcess>
#include <QTreeWidget>
#include <QtDebug>
#include <QObject>
#include <QMenu>
#include <QAction>
#include <qpushbutton.h>

#include "wiitreethread.h"


typedef char			s8;
typedef short			s16;
typedef long			s32;
typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;

namespace Ui
{
    class MainWindow;
}

enum
{
    witNoJob = 0,
    witGetVersion,
    witCopy,
    witIlist,
    witDump
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
//    void contextMenuEvent(QContextMenuEvent *event);
    void dropEvent( QDropEvent *event );
    void dragEnterEvent( QDragEnterEvent *event );

private:
    //mutable QMutex mutex;
    Ui::MainWindow *ui;
    QProcess *witProcess;

    QIcon groupIcon;
    QIcon keyIcon;
    QString filepaths;
    QString isoPath;
    QString witPath;
    QString lastPathLoadedCorrectly;
    QString witVersionString;
    QString witErrorStr;
    QAction *extractAct;
    QAction *replaceAct;
    QStringList extractPaths;
    QStringList replacePaths;
    WiiTreeThread *wiithread;


    u8 gameIOS;
    u8 gameRegion;
    bool undoLastTextOperation;
    u8 witJob;


    void DoIlist();
    QString ItemToFullPath( QTreeWidgetItem * item );
    bool SaveSettings();
    bool LoadSettings();
    void ResizeGuiToLanguage();
    void AbortLoadingGame( QString message );
    void ErrorMessage( QString message );
    int GetRegion();
    int GetIOS();
    void OpenGame();
    int SendWitCommand( QStringList args, int jobType );
    bool FindWit();

    void InsertText( QString s, QString c);



private slots:

    void on_actionWhat_s_This_triggered();
    void on_checkBox_sneek_clicked();
    void on_pushButton_wit_clicked();
    void on_checkBox_defaultRegion_clicked();
    void on_checkBox_defaultIos_clicked();
    void on_actionAbout_Qt_triggered();
    void on_actionAbout_triggered();
    void on_actionSave_As_triggered();
    void on_actionOpen_triggered();
    void on_pushButton_settings_searchPath_clicked();
    void on_pushButton_2_clicked();
    void on_checkBox_7_clicked();
    void on_checkBox_6_clicked();
    void on_pushButton_3_clicked();
    void UpdateOptions();
    void ReadyReadStdOutSlot();
    void ReadyReadStdErrSlot();
    void ProcessFinishedSlot( int, QProcess::ExitStatus );
    void ExtractSlot();
    void ReplaceSlot();

    // get output from a thread used to make a filetree
    void UpdateProgressFromThread( int );
    void ThreadIsDoneRunning( QTreeWidgetItem * );


public slots:
    void ShowMessage( const QString & );

signals:
    // send a kill message to the external running process
    void KillProcess();
};



#endif // MAINWINDOW_H
