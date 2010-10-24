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

#ifndef UNIXFSCHECKER_H
#define UNIXFSCHECKER_H

#include "includes.h"

#define UNIX_FS_CHECKER	    3000

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

};

#endif // UNIXFSCHECKER_H
