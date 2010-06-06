#ifndef FILEFOLDERDIALOG_H
#define FILEFOLDERDIALOG_H
#include <QFileDialog>

class FileFolderDialog: public QFileDialog
{
public:
    FileFolderDialog( QWidget * parent, Qt::WindowFlags flags ):QFileDialog(parent,flags){}
    FileFolderDialog ( QWidget * parent = 0, const QString & caption = QString(), const QString & directory = QString(), const QString & filter = QString() ):
    QFileDialog ( parent ,caption,directory,filter){}
public slots:
    void accept ();
};

#endif // FILEFOLDERDIALOG_H


