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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void AddText( const char in[] );
    void SetStatus( const char str[] );
    void UpdateEffects();

//protected:
//    void contextMenuEvent(QContextMenuEvent *event);

private:
    //mutable QMutex mutex;
    Ui::MainWindow *ui;
    QProcess *witProcess;

    //for the edit tab
    QIcon groupIcon;
    QIcon keyIcon;
    QString filepaths;
    QAction *extractAct;
    QAction *replaceAct;
    QStringList extractPaths;
    QStringList replacePaths;


    int region;
    u8 tmdIOS;
    int witRunning;
    bool undoLastTextOperation;
    int numFstFiles;
    int currentReadLine;

    char id[ 7 ];
    char name[ 0x40 ];
    char regStr[ 5 ];
    bool alreadyGotTitle;

    void ParseFileList();
    void AddItemToTree( const QString s );
    int findItem(  const QString s, QTreeWidgetItem *parent, int startIndex );
    int childCount( QTreeWidgetItem * parent );
    QTreeWidgetItem *childAt(QTreeWidgetItem *parent, int index);
    QTreeWidgetItem *createItem(const QString &text, QTreeWidgetItem *parent, int index);
    QString ItemToFullPath( QTreeWidgetItem * item );



private slots:

    void on_pushButton_2_clicked();
    void on_edit_img_pushButton_clicked();
    void on_lineEdit_editingFinished();
    void on_checkBox_7_clicked();
    void on_checkBox_6_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_toolButton_2_clicked();
    void on_pushButton_4_clicked();
    void on_toolButton_clicked();
    void on_pushButton_3_clicked();
    void UpdateRegStr( int i );
    void UpdateOptions();
    void ReadyReadStdOutSlot();
    void ProcessFinishedSlot( int, QProcess::ExitStatus );
    void ExtractSlot();
    void ReplaceSlot();

public slots:
    void ShowMessage( const QString & );
    void GetThreadDone( int );

signals:
    // send a kill message to the external running process
    void KillProcess();
};



#endif // MAINWINDOW_H
