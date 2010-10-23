#ifndef CUSTOMMDIITEM_H
#define CUSTOMMDIITEM_H

#include <QObject>
#include <QMdiSubWindow>

enum
{
    mdiAny = -1,
    mdiNone = 0,
    mdiPartition,
    mdiGame,
    mdiWiiTDB,
    mdiCovers
};

class CustomMdiItem : public QMdiSubWindow
{
    Q_OBJECT
public:
    CustomMdiItem( QWidget * parent = 0, Qt::WindowFlags flags = 0, QString windowTitle = QString() );

    //provide a way to distinguish mdi windows
    QString GetTitle();
    void setWindowTitle( QString windowTitle );
    int type;

private:
    QString title;

signals:
    //provide a way for other stuff to know this window is closing
    void AboutToClose( QString title, QPoint pos, QSize size, int type );

public slots:

protected:
    void closeEvent( QCloseEvent * closeEvent );

};

#endif // CUSTOMMDIITEM_H
