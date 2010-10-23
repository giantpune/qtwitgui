#ifndef WWTHANDLER_H
#define WWTHANDLER_H

#include "includes.h"

enum
{
    wwtNoJob = 2000,
    wwtGetVersion,
    wwtFind,
    wwtAdd,
    wwtRemove
};

class WwtHandler : public QObject
{
    Q_OBJECT

public:
    WwtHandler( QObject *parent = 0, bool root = false );
    ~WwtHandler();

    void SetRunAsRoot( bool root = true );
    void GetPartitions();

    //block the current thread and wait for wit to finish
    bool Wait( int msecs = 30000 );
    void Kill();

    void RunJob( QStringList args, int jobType );

private:
    QProcess *process;
    int wwtJob;

    QString errStr;
    QString stdStr;

    QString currJobText;

    bool runAsRoot;
    bool requestedPassword;

    QString GetWwtPath();

signals:
    void KillProcess();
    void SendFatalErr( QString, int );
    //void SendStdErr( QString );
    //void SendStdOut( QString );
    void SendProgress( int );
    void SendMessageForStatusBar( QString );
    void SendPartitionList( QStringList );
    void RequestPassword();
    void SendJobDone( int );

public slots:
    void PasswordIsEntered();

private slots:
    void ReadyReadStdOutSlot();
    void ReadyReadStdErrSlot();
    void ProcessFinishedSlot( int, QProcess::ExitStatus );

};



#endif // WWTHANDLER_H
