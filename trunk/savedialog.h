#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include "includes.h"

namespace Ui {
    class SaveDialog;
}

class SaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveDialog(QWidget *parent = 0, const QString boldText = QString(), const QString lightText = QString(), const QString details = QString() );
    ~SaveDialog();

private:
    Ui::SaveDialog *ui;
    void ToggleDetails();
    QSize bigSize;
    QSize smallSize;
    bool detailsVisible;
    void SaveSettings();

protected:
    //void reject();

private slots:
    void on_pushButton_details_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_ok_clicked();
};

#endif // SAVEDIALOG_H
