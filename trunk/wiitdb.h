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
    bool LoadFile( QString name );

    //date and time of the xml file ( from the version attribute )
    QDate xmlDate;
    QTime xmlTime;

    //number of games in the loaded xml
    int xmlGameNum;

    //populates the gui with data from a given id
    bool LoadGameFromID( QString id );

    //access function for getting a name without setting all the other variables to the new game
    QString NameFromID( QString id );

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

private:
    QFile file;//if reading from a unzipped xml, use this
    QBuffer buf;//store the unziped xml here in memory
    QDomDocument domDocument;
    QDomElement GameFromID( QString id );
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

signals:
    void SendError( QString title, QString detials );

public slots:

};

//this is the only instance used for the whole gui.
extern WiiTDB *wiiTDB;

#endif // WIITDB_H
