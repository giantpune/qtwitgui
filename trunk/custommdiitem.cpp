#include "includes.h"
#include "custommdiitem.h"

CustomMdiItem::CustomMdiItem( QWidget * parent, Qt::WindowFlags flags, QString windowTitle ) : QMdiSubWindow( parent, flags )
{
    title = windowTitle;
    setAttribute( Qt::WA_DeleteOnClose );
    QWidget::setWindowTitle( windowTitle );
}

void CustomMdiItem::closeEvent( QCloseEvent * closeEvent )
{
    if( isMinimized () )
    {

    }
    else if( isMaximized () )
	emit AboutToClose( title, pos(), QSize( 999, 999 ), type );
    else
	emit AboutToClose( title, pos(), size(), type );

    QWidget::closeEvent( closeEvent );
}

QString CustomMdiItem::GetTitle()
{
    return title;
}

void CustomMdiItem::setWindowTitle( QString windowTitle )
{
    title = windowTitle;
    QWidget::setWindowTitle( windowTitle );
}

