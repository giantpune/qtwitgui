#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
    class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordDialog(QWidget *parent = 0);
    ~PasswordDialog();

private:
    Ui::PasswordDialog *ui;

private slots:
    void on_pushButton_ok_clicked();

signals:
    void SendPassword( QString );
};

#endif // PASSWORDDIALOG_H
