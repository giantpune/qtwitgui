#include "includes.h"
#include "wiitdbwindow.h"
#include "ui_wiitdbwindow.h"

WiiTDBWindow::WiiTDBWindow(QWidget *parent) : QWidget(parent), ui(new Ui::WiiTDBWindow)
{
    setAttribute( Qt::WA_DeleteOnClose );
    ui->setupUi(this);
    ClearGui();

    ui->graphicsView->setScene( &gv );
    ui->graphicsView->setAlignment( Qt::AlignRight );
    ui->graphicsView->setRenderHint( QPainter::Antialiasing );

    connect( wiiTDB, SIGNAL( SendError( QString, QString ) ), this, SLOT( ReceiveErrorFromWiiTDB( QString, QString ) ) );
}

WiiTDBWindow::~WiiTDBWindow()
{
    emit Destroyed();
    delete ui;
}

QString WiiTDBWindow::GetVersion()
{
    return wiiTDB->xmlDate.toString( "dd/MM/yyyy" ) + " : " + wiiTDB->xmlTime.toString( "hh:mm:ss" );
}

int WiiTDBWindow::GameCount()
{
    return wiiTDB->xmlGameNum;
}

void WiiTDBWindow::SetFile( QString path )
{
    wiiTDB->LoadFile( path );
}

void WiiTDBWindow::AddImage( QPixmap pm )
{
    QGraphicsPixmapItem *pmi = new QGraphicsPixmapItem( pm );
    gv.addItem( pmi );
}

void WiiTDBWindow::AddImage( QString str )
{
    if( !QFile::exists( str ) )
	qDebug() << "bad filename:" << ui->label_id->text() << str;
    QGraphicsPixmapItem *pmi = new QGraphicsPixmapItem( QPixmap( str ) );
    gv.addItem( pmi );
}

void WiiTDBWindow::ClearGui()
{
    ui->label_date->clear();
    ui->label_developer->clear();
    ui->label_genre->clear();
    ui->label_id->clear();
    ui->label_languages->clear();
    ui->label_publisher->clear();
    ui->label_region->clear();
    ui->label_title->clear();
    ui->label_type->clear();
    ui->label_wifiFeatures->clear();
    ui->textBrowser_synopsis->clear();
    foreach( QGraphicsItem *i, gv.items() )
    {
	gv.removeItem( i );
	delete( i );
    }
}

void WiiTDBWindow::LoadGameFromID( QString id )
{
    ClearGui();
    ui->label_id->setText( id );

    if( !wiiTDB->LoadGameFromID( id ) )
    {
	return;
    }

    ui->label_date->setText( wiiTDB->releaseDate.toString( "ddd MMMM d yyyy" ) );
    ui->label_developer->setText( wiiTDB->developer );
    ui->label_genre->setText( wiiTDB->genre );
    ui->label_languages->setText( wiiTDB->languages );
    ui->label_publisher->setText( wiiTDB->publisher );
    ui->label_region->setText( wiiTDB->region );
    ui->label_title->setText( wiiTDB->title );
    ui->label_type->setText( wiiTDB->type );
    ui->label_wifiFeatures->setText( wiiTDB->wifiFeatures );
    ui->textBrowser_synopsis->setText( wiiTDB->synopsis );


    //add all the pretty icons

    //wiimote & num players
    QString str;
    if( wiiTDB->type == "GameCube" )
	QTextStream( &str ) << ":/images/gc" << wiiTDB->inputPlayers << ".png";
    else
	QTextStream( &str ) << ":/images/Wiimote" << wiiTDB->inputPlayers << ".png";
    AddImage( str );

    //other accessories
    QMap<QString, bool> map = wiiTDB->inputControlers;
    QMapIterator<QString, bool> i( map );
    while( i.hasNext() )
    {
	 i.next();
	 str = i.key();
	 if( str == "wiimote" )
	     continue;
	 else if( str == "nunchuk" )
	 {
	     AddImage( i.value() ? ":/images/nunchukR.png" : ":/images/nunchuk.png" );
	 }
	 else if( str == "motionplus" )
	 {
	     AddImage( i.value() ? ":/images/motionplusR.png" : ":/images/motionplus.png" );
	 }
	 else if( str == "gamecube" && wiiTDB->type != "GameCube" )
	 {
	     AddImage( i.value() ? ":/images/gamecubeR.png" : ":/images/gamecube.png" );
	 }
	 else if( str == "nintendods" )
	 {
	     AddImage( i.value() ? ":/images/nintendodsR.png" : ":/images/nintendods.png" );
	 }
	 else if( str == "classiccontroller" )
	 {
	     AddImage( i.value() ? ":/images/classiccontrollerR.png" : ":/images/classiccontroller.png" );
	 }
	 else if( str == "logitech force feedback" )
	 {
	     AddImage( i.value() ? ":/images/forceFeedbackR.png" : ":/images/forceFeedback.png" );
	 }
	 else if( str == "wheel" )
	 {
	     AddImage( i.value() ? ":/images/wheelR.png" : ":/images/wheel.png" );
	 }
	 else if( str == "zapper" )
	 {
	     AddImage( i.value() ? ":/images/zapperR.png" : ":/images/zapper.png" );
	 }
	 else if( str == "balanceboard" )
	 {
	     AddImage( i.value() ? ":/images/balanceboardR.png" : ":/images/balanceboard.png" );
	 }
	 else if( str == "wiispeak" )
	 {
	     AddImage( i.value() ? ":/images/wiispeakR.png" : ":/images/wiispeak.png" );
	 }
	 else if( str == "microphone" )
	 {
	     AddImage( i.value() ? ":/images/microphoneR.png" : ":/images/microphone.png" );
	 }
	 else if( str == "guitar" )
	 {
	     AddImage( i.value() ? ":/images/guitarR.png" : ":/images/guitar.png" );
	 }
	 else if( str == "drums" )
	 {
	     AddImage( i.value() ? ":/images/drumsR.png" : ":/images/drums.png" );
	 }
	 else if( str == "dancepad" )
	 {
	     AddImage( i.value() ? ":/images/dancepadR.png" : ":/images/dancepad.png" );
	 }
	 else if( str == "gameboy advance" )
	 {
	     AddImage( ":/images/gba.png" );//not using the gbaR.png.  i dont think there should be any games that actually require it
	 }
	 else if( str == "dk bongo" )
	 //else if( str == "dk bongo" || str == "bongo dk" )
	 {
	     AddImage( i.value() ? ":/images/bongosR.png" : ":/images/bongos.png" );
	 }
	 else if( str != "gamecube" )
	     qDebug() << "unhandled shit::" << id << i.key() << ": " << i.value();
    }
    //wi-fi players
    int n = wiiTDB->wifiPlayers;
    if( n )
    {
	str.clear();
	QTextStream( &str ) << ":/images/wifi" << n << ".png";
	AddImage( str );
    }
    //rating
    str = wiiTDB->ratingType;
    if( !str.isEmpty() )
    {
	AddImage( ":/images/" + str.toLower() + "_" + wiiTDB->ratingValue.toLower() + ".png" );
    }
    else AddImage( ":/images/norating.png" );


    // align all the images right to left, starting with the last one
    int hspace = 4;
    int ho = ui->graphicsView->width();
    for( int j = 0; j < gv.items().size(); j++ )
    {
	QGraphicsItem *i = gv.items().at( j );
	ho -= i->boundingRect().width() + hspace;

	i->setX( ho );
    }
}

//get an error from the wiitdb and display it as an error window
void WiiTDBWindow::ReceiveErrorFromWiiTDB( QString title, QString detail )
{
    QMessageBox::information( this, title, detail );
}


