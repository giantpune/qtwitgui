#ifndef OSXFS_H
#define OSXFS_H

#include "includes.h"
class OSxFs : public QObject
{
Q_OBJECT
public:
    explicit OSxFs(QObject *parent = 0);
    static QString GetFilesystem( QString path );

signals:

public slots:

};

#endif // OSXFS_H
