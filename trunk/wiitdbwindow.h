#ifndef WIITDBWINDOW_H
#define WIITDBWINDOW_H


#include "includes.h"
#include "wiitdb.h"

namespace Ui {
    class WiiTDBWindow;
}

class WiiTDBWindow : public QWidget
{
    Q_OBJECT

public:
    explicit WiiTDBWindow(QWidget *parent = 0);
    ~WiiTDBWindow();
    void SetFile( QString path );

    QString GetVersion();
    int GameCount();



private:
    Ui::WiiTDBWindow *ui;
    QGraphicsScene gv;

    void AddImage( QPixmap pm );
    void AddImage( QString str );
    //WiiTDB wiiTDB;
    void ClearGui();

public slots:
    void LoadGameFromID( QString id );

private slots:
    void ReceiveErrorFromWiiTDB( QString title, QString detail );

signals:
    void Destroyed();
};

#endif // WIITDBWINDOW_H
