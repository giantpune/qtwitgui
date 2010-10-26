#ifndef FSINFO_H
#define FSINFO_H


#include "includes.h"
class FsInfo : public QObject
{
Q_OBJECT
public:
    explicit FsInfo(QObject *parent = 0);

    static QString GetFilesystem( QString path );

    //only used for vindows stuff
    static bool Check();
    static QString ToWinPath( QString cygPath, bool *ok );
    static QString ToCygPath( QString winPath, bool *ok );

signals:

public slots:

};

#endif // FSINFO_H
