#include "dvdselectdialog.h"
#include "ui_dvdselectdialog.h"
#include "fsinfo.h"

DvdSelectDialog::DvdSelectDialog(QWidget *parent) : QDialog(parent), ui(new Ui::DvdSelectDialog)
{
    ui->setupUi(this);
    QTimer::singleShot( 100, this, SLOT( on_pushButton_refresh_clicked() ) );
}

DvdSelectDialog::~DvdSelectDialog()
{
    delete ui;
}

void DvdSelectDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QStringList DvdSelectDialog::GetDvdToOpen( QWidget *parent )
{
    DvdSelectDialog d( parent );
    if( d.exec() )
        return d.ret;

    return QStringList();
}

void DvdSelectDialog::on_buttonBox_accepted()
{
    ret.clear();
    QList<QListWidgetItem *> selected = ui->listWidget->selectedItems();
    int size = selected.size();
    for( int i = 0; i < size; i++ )
    {
        ret << selected.at( i )->text();
    }
}

void DvdSelectDialog::on_pushButton_refresh_clicked()
{
    ui->label_info->setText( tr( "Checking available DVD drives..." ) );

    QStringList dvds = FsInfo::GetDvdDrives();
    if( !dvds.size() )
    {
        ui->label_info->setText( tr( "No DVD drives found" ) );
        return;
    }

    //clear all entries in the list
    while( ui->listWidget->count() )
    {
        QListWidgetItem *item = ui->listWidget->takeItem( 0 );
        delete item;
    }

    ui->listWidget->addItems( dvds );
    ui->label_info->setText( tr( "Select a Drive to open" ) );
}
