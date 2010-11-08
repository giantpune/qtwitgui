/************************************************************************************
*
*   - QtWitGui -				2010 giantpune
*
*   the multilingual, multiplatform, multiformat gui for messing with
*   Wii game images.
*
*   This software comes to you with a GPLv3 license.
*   http://www.gnu.org/licenses/gpl-3.0.html
*
*   Basically you are free to modify this code, distribute it, use it in
*   other projects, or anything along those lines.  Just make sure that any
*   derivative work gets the same license.  And don't remove this notice from
*   the derivative work.
*
*   And please, don't be a douche.  If you borrow code from here, don't claim
*   you wrote it.  Share your source code with others.  Even if you are
*   charging a fee for the binaries, let others read the code as somebody has
*   done for you.
*
*************************************************************************************/

#ifndef WIITDB_H
#define WIITDB_H

#include "includes.h"

class WiiTDB : public QObject
{
    Q_OBJECT
public:
    explicit WiiTDB( QObject *parent = 0 );
    ~WiiTDB();

    //load up a wiitdb.xml file ( .zip not supported yet )
    bool LoadFile( const QString &name );

    //date and time of the xml file ( from the version attribute )
    QDate xmlDate;
    QTime xmlTime;

    //number of games in the loaded xml
    int xmlGameNum;

    //populates the strings with data from a given id
    bool LoadGameFromID( const QString &id );

    //access function for getting a name without setting all the other variables to the new game
    QString NameFromID( const QString &id );

    //variables for exposing game info
    QString id_loaded;
    QString title;
    QString synopsis;
    QString type;
    QString region;
    QString languages;
    QString developer;
    QString publisher;
    QDate releaseDate;//release date is inaccurate in that it will substitute "1" for missing day/month values
    QString genre;
    QString ratingType;
    QString ratingValue;
    int wifiPlayers;
    QString wifiFeatures;
    int inputPlayers;
    QMap<QString, bool> inputControlers;

    QList< QTreeWidgetItem * >Search( const QString &id = QString(), const QString &name = QString(), const QString &players = QString(),\
			    int playerCmpType = -1, const QString &wifiPlayers = QString(), int wifiCmpType = -1 );

private:
    QFile file;//if reading from a unzipped xml, use this
    QBuffer buf;//store the unziped xml here in memory
    QDomDocument domDocument;
    QDomElement GameFromID( const QString &id );
    QString localeStr;

    void ClearGame();
    //returns localized strings if they exist.
    //they expect the "<game>" element
    QString NameFromGameElement( QDomElement parent );
    QString SynopsisFromGameElement( QDomElement parent );
    QString TypeFromGameElement( QDomElement parent );
    QString RegionFromGameElement( QDomElement parent );
    QString LanguagesFromGameElement( QDomElement parent );
    QString DeveloperFromGameElement( QDomElement parent );
    QString PublisherFromGameElement( QDomElement parent );
    QDate DateFromGameElement( QDomElement parent );
    QString GenreFromGameElement( QDomElement parent );
    QString RatingValueFromGameElement( QDomElement parent );
    QString RatingTypeFromGameElement( QDomElement parent );
    int WifiPlayersGameElement( QDomElement parent );
    QString WifiFeaturesFromGameElement( QDomElement parent );
    int InputPlayersFromGameElement( QDomElement parent );
    QMap<QString, bool> InputControllersFromGameElement( QDomElement parent );

    //check if text matches a qregex
    bool CheckRegEx( const QString &text, const QRegExp &rx );

    //check if a # of players falls within the searched rules
    bool CheckPlayerRule( int num, int cmpType, int cmpval );

signals:
    void SendError( QString title, QString detials );

public slots:

};

//this is the only instance used for the whole gui.
extern WiiTDB *wiiTDB;

#endif // WIITDB_H
