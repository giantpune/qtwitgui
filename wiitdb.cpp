#include "wiitdb.h"
#ifndef Q_WS_MAC
#include "unzip/unzip.h"
#endif
#include "includes.h"

#include <QIODevice>

WiiTDB *wiiTDB;

WiiTDB::WiiTDB( QObject *parent ) : QObject( parent )
{
    //grab the language code to use for localized text
    localeStr = QLocale::system().name();
    if( localeStr.startsWith( "zh" ) )//some sort of chinese
    {
	if( localeStr == "zh_TW" )//taiwan
	    localeStr = "ZHTW";

	else// make all other chinese use the china version
	    localeStr = "ZHCH";
    }
    else // all other languages
    {
	localeStr.resize( 2 );
	localeStr = localeStr.toUpper();
    }

    //load the default wiitdb.xml
    //LoadFile( ":/wiitdb.xml" );
}

WiiTDB::~WiiTDB()
{
    if( file.isOpen() )
	file.close();

    if( buf.isOpen() )
	buf.close();
}

//load the wiitdb.xml file
bool WiiTDB::LoadFile( const QString &name )
{
    //qDebug() << "WiiTDB::LoadFile" << name;
    if( file.isOpen() )
	file.close();

    if( buf.isOpen() )
	buf.close();

    QString errorStr;
    int errorLine;
    int errorColumn;

    title = tr( "No WiiTDB available" );
    if( name.endsWith( ".xml", Qt::CaseInsensitive ) )
    {
	//open the file
	file.setFileName( name );
	if( !file.open(QFile::ReadOnly | QFile::Text ) )
	{
	    emit SendError( tr("WiiTDB Error"), tr("Cannot read file.") );
	    return false;
	}
	//point the dom to the file
	if( !domDocument.setContent( &file, true, &errorStr, &errorLine, &errorColumn ) )
	{
	    emit SendError( tr("WiiTDB Error"), tr("Parse error at line %1, column %2:\n%3").arg( errorLine ).arg( errorColumn ).arg( errorStr ) );
	    return false;
	}
    }
    else if( name.endsWith( ".zip", Qt::CaseInsensitive ) )
    {
#ifdef Q_WS_MAC
        emit SendError( tr("WiiTDB Error"), tr("Zip files are not supported in the OSx version of this program yet") );
        return false;
#else
	if( !QFile::exists( name ) )
	{
		emit SendError( tr("WiiTDB Error"), tr("Cannot find %1.").arg( name ) );
		return false;
	}

	UnZip::ErrorCode ec;
	UnZip uz;

	//try to open the zip archize
	ec = uz.openArchive( name );
	if (ec != UnZip::Ok)
	{
		emit SendError( tr("WiiTDB Error"), QString( "Failed to open archive: " ) + uz.formatError(ec).toAscii() );
		return false;
	}

	//see if it contains the correct file
	if( !uz.contains( "wiitdb.xml" ) )
	{
	    emit SendError( tr("WiiTDB Error"), tr( "wiitdb.xml not found in the archive." ) );
	    return false;
	}

	//open the memory oidevice
	if( !buf.open( QIODevice::ReadWrite ) )
	{
	    emit SendError( tr("WiiTDB Error"), tr( "Memory error." ) );
	    return false;
	}

	//extract that file to our memory buffer
	ec = uz.extractFile( "wiitdb.xml", &buf);
	if (ec != UnZip::Ok)
	{
		emit SendError( tr("WiiTDB Error"), QString( "Extraction failed: " ) + uz.formatError(ec).toAscii() );
		uz.closeArchive();
		return false;
	}

	//rewind back to the beginning
	buf.seek( 0 );

	//point the dom to the buffer
	if( !domDocument.setContent( &buf, true, &errorStr, &errorLine, &errorColumn ) )
	{
	    emit SendError( tr("WiiTDB Error"), tr("Parse error at line %1, column %2:\n%3").arg( errorLine ).arg( errorColumn ).arg( errorStr ) );
	    return false;
	}
#endif
    }
    else
    {
	qDebug() << name;
	emit SendError( tr("WiiTDB Error"), tr("Only .zip and .xml files are supported.") );
	return false;
    }
    //find and check root tag
    QDomElement root = domDocument.documentElement();
    if( root.tagName() != "datafile" )
    {
	emit SendError( tr("WiiTDB Error"), tr("The file is missing the \"datafile\" root tag.") );
	return false;
    }

    QDomElement child = root.firstChildElement( "WiiTDB" );
    if( child.isNull() || !child.hasAttribute( "version" ) || !child.hasAttribute( "games" ) )
    {
	emit SendError( tr("WiiTDB Error"), tr("Invalid \"WiiTDB\" tag.") );
	return false;
    }

    bool ok;
    xmlGameNum = child.attribute( "games" ).toInt( &ok );
    if( !ok )
    {
	emit SendError( tr("WiiTDB Error"), tr("Invalid attribute in the \"WiiTDB\" tag.") );
	return false;
    }
    title.clear();
    ClearGame();

    QString ver = child.attribute( "version" );
    ver.resize( 8 );
    xmlDate = QDate::fromString( ver, "yyyyMMdd");

    ver = child.attribute( "version" );
    ver.remove( 0, 8 );
    xmlTime = QTime::fromString( ver, "hhmmss" );

    return true;
}

bool WiiTDB::LoadGameFromID( const QString &id )
{
    if( !file.isOpen() && !buf.isOpen() )
	return false;
    if( id == id_loaded )
	return true;
    ClearGame();
    QDomElement e = GameFromID( id );
    if( e.isNull() )
    {
	if( id.at( 0 ) == QChar( '0' ) || id.at( 0 ) == QChar( '1' ) )//if this game starts with 0 or 1 then it is an autoboot game.  try to change it to R or S and see if the game exists
	{
	    QString i = id;
	    i[ 0 ] = QChar( 'R' );
	    e = GameFromID( i );
	    if( e.isNull() )
	    {
		i[ 0 ] = QChar( 'S' );
		e = GameFromID( i );
		if( e.isNull() )
		{
		    return false;
		}
		return false;
	    }
	}
	return false;
    }
    id_loaded = id;
    title = NameFromGameElement( e );
    synopsis = SynopsisFromGameElement( e );
    type = TypeFromGameElement( e );
    region = RegionFromGameElement( e );
    languages = LanguagesFromGameElement( e );
    developer = DeveloperFromGameElement( e );
    publisher = PublisherFromGameElement( e );
    releaseDate = DateFromGameElement( e );
    genre = GenreFromGameElement( e );
    ratingType = RatingTypeFromGameElement( e );
    ratingValue = RatingValueFromGameElement( e );
    wifiPlayers = WifiPlayersGameElement( e );
    wifiFeatures = WifiFeaturesFromGameElement( e );
    inputPlayers = InputPlayersFromGameElement( e );
    inputControlers = InputControllersFromGameElement( e );

    return true;
}

void WiiTDB::ClearGame()
{
    id_loaded.clear();
    title.clear();
    synopsis.clear();
    type.clear();
    region.clear();
    languages.clear();
    developer.clear();
    publisher.clear();
    releaseDate = QDate();
    genre.clear();
    ratingType.clear();
    ratingValue.clear();
    wifiPlayers = 0;
    wifiFeatures.clear();
    inputPlayers = 0;
    inputControlers.clear();
}

//get the "<game>" tag for a give ID
QDomElement WiiTDB::GameFromID( const QString &id )
{
    QDomElement root = domDocument.documentElement();
    QDomElement child = root.firstChildElement( "game" );
    if( child.isNull() )
    {
	emit SendError( tr("WiiTDB Error"), tr("No \"game\" tag found.") );
	return QDomElement();
    }

    while( !child.isNull() )
    {
	QDomElement idNode = child.firstChildElement( "id" );
	if( !idNode.isNull() )
	{
	    if( idNode.text() == id )
		return child;
	}
	child = child.nextSiblingElement( "game" );
    }

    return QDomElement();
}

//get a game from an ID
QString WiiTDB::NameFromID( const QString &id )
{
    if( !file.isOpen() )
	return QString();

    QDomElement e = GameFromID( id );
    if( e.isNull() )
	return QString();

    return NameFromGameElement( e );
}

//find shit from the game element
QString WiiTDB::NameFromGameElement( QDomElement parent )
{
    QDomElement localeElement = parent.firstChildElement( "locale" );
    while( !localeElement.isNull() )
    {
	if( localeElement.hasAttribute( "lang" ) && localeElement.attribute( "lang" ) == localeStr )
	{
	    QDomElement titleElement = localeElement.firstChildElement( "title" );
	    if( !titleElement.isNull() && !titleElement.text().isEmpty() )
	    {
		return titleElement.text();
	    }
	}
	localeElement = localeElement.nextSiblingElement( "locale" );
    }
    //localized language not found.  look for generic one
    if( parent.hasAttribute( "name") && !parent.attribute( "name" ).isEmpty() )
	return parent.attribute( "name" );

    return QString();
}
QString WiiTDB::SynopsisFromGameElement( QDomElement parent )
{
    QDomElement localeElement = parent.firstChildElement( "locale" );
    QString enSynopsis;
    while( !localeElement.isNull() )
    {
	if( localeElement.hasAttribute( "lang" ) )
	{
	    if( localeElement.attribute( "lang" ) == localeStr )
	    {
		QDomElement titleElement = localeElement.firstChildElement( "synopsis" );
		if( !titleElement.isNull() && !titleElement.text().isEmpty() )
		{
		    return titleElement.text();
		}
	    }
	    if( localeElement.attribute( "lang" ) == "EN" )
	    {
		QDomElement titleElement = localeElement.firstChildElement( "synopsis" );
		if( !titleElement.isNull() && !titleElement.text().isEmpty() )
		{
		    enSynopsis = titleElement.text();
		}
	    }
	}
	localeElement = localeElement.nextSiblingElement( "locale" );
    }
    //localized language not found.  return the english one
    return enSynopsis;
}
QString WiiTDB::TypeFromGameElement( QDomElement parent )
{
    QString ret;
    QDomElement e = parent.firstChildElement( "type" );
    if( !e.isNull() )
	ret = e.text();

    return ret;
}
QString WiiTDB::RegionFromGameElement( QDomElement parent )
{
    QString ret;
    QDomElement e = parent.firstChildElement( "region" );
    if( !e.isNull() )
	ret = e.text();

    return ret;
}
QString WiiTDB::LanguagesFromGameElement( QDomElement parent )
{
    QString ret;
    QDomElement e = parent.firstChildElement( "languages" );
    if( !e.isNull() )
	ret = e.text();

    return ret;
}
QString WiiTDB::DeveloperFromGameElement( QDomElement parent )
{
    QString ret;
    QDomElement e = parent.firstChildElement( "developer" );
    if( !e.isNull() )
	ret = e.text();

    return ret;
}
QString WiiTDB::PublisherFromGameElement( QDomElement parent )
{
    QString ret;
    QDomElement e = parent.firstChildElement( "publisher" );
    if( !e.isNull() )
	ret = e.text();

    return ret;
}
QDate WiiTDB::DateFromGameElement( QDomElement parent )
{
    QDate ret;
    QString str;
    QDomElement e = parent.firstChildElement( "date" );
    if( !e.isNull() )
    {
	if( e.hasAttribute( "year" ) )
	    str += e.attribute( "year" );
	else// no release year, just bail out
	    return ret;

	if( e.hasAttribute( "month" ) )
	    str += e.attribute( "month" ).rightJustified( 2, '0' );
	else//no month.  just make it january
	    str += "01";

	if( e.hasAttribute( "day" ) )
	    str += e.attribute( "day" ).rightJustified( 2, '0' );
	else//no day.  default to the first of the month
	    str += "01";
    }
    ret = QDate::fromString( str, "yyyyMMdd" );

    return ret;
}
QString WiiTDB::GenreFromGameElement( QDomElement parent )
{
    QString ret;
    QDomElement e = parent.firstChildElement( "genre" );
    if( !e.isNull() )
	ret = e.text();

    return ret;
}
QString WiiTDB::RatingTypeFromGameElement( QDomElement parent )
{
    QString ret;
    QDomElement e = parent.firstChildElement( "rating" );
    if( !e.isNull() && e.hasAttribute( "type" ) )
	ret = e.attribute( "type" );

    return ret;
}
QString WiiTDB::RatingValueFromGameElement( QDomElement parent )
{
    QString ret;
    QDomElement e = parent.firstChildElement( "rating" );
    if( !e.isNull() && e.hasAttribute( "value" ) )
	ret = e.attribute( "value" );

    return ret;
}
int WiiTDB::WifiPlayersGameElement( QDomElement parent )
{
    int ret = 0;
    bool ok;
    QString str;
    QDomElement e = parent.firstChildElement( "wi-fi" );
    if( !e.isNull() && e.hasAttribute( "players" ) )
    {
	str = e.attribute( "players" );
	ret = str.toInt( &ok, 10 );
	if( ok )
	    return ret;
    }

    return 0;
}
QString WiiTDB::WifiFeaturesFromGameElement( QDomElement parent )
{
    QString ret;
    QDomElement e = parent.firstChildElement( "wi-fi" );
    if( !e.isNull() )
    {
	QDomElement f = e.firstChildElement( "feature" );
	while( 1 )
	{
	    ret += f.text();
	    f = f.nextSiblingElement( "feature" );
	    if( !f.isNull() )
		ret += ", ";
	    else
		break;
	}
    }
    return ret;
}
int WiiTDB::InputPlayersFromGameElement( QDomElement parent )
{
    int ret = 0;
    bool ok;
    QString str;
    QDomElement e = parent.firstChildElement( "input" );
    if( !e.isNull() && e.hasAttribute( "players" ) )
    {
	str = e.attribute( "players" );
	ret = str.toInt( &ok, 10 );
	if( ok )
	    return ret;
    }

    return 1;
}
QMap<QString, bool> WiiTDB::InputControllersFromGameElement( QDomElement parent )
{
    QMap<QString, bool> ret;
    QDomElement e = parent.firstChildElement( "input" );
    if( !e.isNull() )
    {
	QDomElement c = e.firstChildElement( "control" );
	while( !c.isNull() )
	{
	    bool required = false;
	    QString type;
	    if( c.hasAttribute( "type") )
	    {
		type = c.attribute( "type" );
		if( c.hasAttribute( "required" ) )
		    required = c.attribute( "required" ) == "true";

		ret.insert( type, required );
	    }
	    c = c.nextSiblingElement( "control" );
	}
    }
    return ret;
}


QList< QTreeWidgetItem * >WiiTDB::Search( const QString &id, const QString &name, const QString &players, int playerCmpType, \
					  const QString &wifiPlayers, int wifiCmpType )
{
    QDomElement root = domDocument.documentElement();
    QDomElement child = root.firstChildElement( "game" );
    if( child.isNull() )
    {
	emit SendError( tr("WiiTDB Error"), tr("No \"game\" tag found.") );
	return QList< QTreeWidgetItem * >();
    }

    QList< QTreeWidgetItem * >ret;

    //only do these conversions 1 time
    QRegExp rxTitle( name, Qt::CaseInsensitive );
    QRegExp rxID( id, Qt::CaseInsensitive );
    bool okPlr = true;
    int plr = -1;
    if( !players.isEmpty() )
	plr = players.toInt( &okPlr );
    if( !okPlr )
	return ret;
    int plrWiFi = -1;
    if( !wifiPlayers.isEmpty() )
	plrWiFi = wifiPlayers.toInt( &okPlr );
    if( !okPlr )
	return ret;

    while( !child.isNull() )
    {
	QDomElement idNode = child.firstChildElement( "id" );
	if( !idNode.isNull() )
	{
	    QString curID = idNode.text();
	    if( id.isEmpty() || CheckRegEx( curID, rxID ) )
	    {
		QString title = NameFromGameElement( child );
		if( name.isEmpty() || CheckRegEx( title, rxTitle ) )
		{
		    int curPly = InputPlayersFromGameElement( child );
		    if( players.isEmpty() || CheckPlayerRule( curPly, playerCmpType, plr ) )
		    {
			int curWPly = WifiPlayersGameElement( child );
			if( wifiPlayers.isEmpty() || CheckPlayerRule( curWPly, wifiCmpType, plrWiFi ) )
			{
			    QString plyStr = QString( "%1" ).arg( curPly );
			    QString plyWStr = QString( "%1" ).arg( curWPly );
			    QTreeWidgetItem *item = new QTreeWidgetItem( QStringList() << curID << title << plyStr << plyWStr );
			    ret << item;
			}
		    }
		}
	    }
	}
	child = child.nextSiblingElement( "game" );
    }
    return ret;
}

bool WiiTDB::CheckRegEx( const QString &text, const QRegExp &rx )
{
    if( text.isEmpty() )
	return false;

    if( !rx.isValid() )
    {
	qDebug() << rx.pattern() << "invalid";
	return text.contains( rx.pattern(), Qt::CaseInsensitive );
    }

    return text.contains( rx );
}

bool WiiTDB::CheckPlayerRule( int num, int cmpType, int cmpval )
{
    switch( cmpType )
    {
    case 0:
	return num < cmpval;
	break;
    case 1:
	return num <= cmpval;
	break;
    case 2:
	return num == cmpval;
	break;
    case 3:
	return num >= cmpval;
	break;
    case 4:
	return num > cmpval;
	break;
    case 5:
	return num != cmpval;
	break;
    }
    //shouldnt happen
    return false;

}



