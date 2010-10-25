#ifndef WINDOWSFSSTUFF_H
#define WINDOWSFSSTUFF_H

#include "includes.h"
class WindowsFsStuff : public QObject
{
Q_OBJECT
public:
    explicit WindowsFsStuff(QObject *parent = 0);
    static bool Check();
    static QString ToWinPath( QString cygPath, bool *ok );
    static QString ToCygPath( QString winPath, bool *ok );
    static QString GetFilesystem( QString path );

signals:

public slots:

};

/*
class UnixFsChecker : public QObject
{
    Q_OBJECT

public:
    UnixFsChecker( QObject *parent = 0 );
    ~UnixFsChecker();

    void SetRunAsRoot( bool root );

    //block the current thread and wait for wit to finish
    bool Wait( int msecs = 30000 );
    void Kill();

    void GetFsTypes( QStringList parts );

private:
    QProcess *process;

    QString errStr;
    QStringList partsMounted;//holds the paths we were passed.  we resolve them using the mount file,
    QStringList partsResolved;//get the filesystem of the resolved partition, and combine the mounted path with the filesystem of the resolved partitoin
    //QString stdStr;

    void RunJob( QStringList args );

    bool running;
    bool runAsRoot;

signals:
    void KillProcess();
    void SendFatalErr( QString, int );
    void SendStdErr( QString );
    void SendPartitionList( QStringList );
    void RequestPassword();

public slots:
    void PasswordIsEntered();

private slots:
    void ReadyReadStdErrSlot();
    void ProcessFinishedSlot( int, QProcess::ExitStatus );

};*/
#endif // WINDOWSFSSTUFF_H
