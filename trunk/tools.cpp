#include "tools.h"

QString settingsPath;

QString SizeTextGiB( QString bytes )
{
    QString ret;
    bool ok = false;
    qint64 i = bytes.toDouble( &ok );
    if( !ok )
	return ret;

    float f = (float)( (float)i / _GiB_ );
    QTextStream t( &ret );
    t.setRealNumberNotation( QTextStream::FixedNotation );
    t.setRealNumberPrecision( 2 );
    t << f;

    return ret;
}

QString gameID( QTreeWidgetItem* item )
{
    return item->text( GAME_ID_COLUMN );
}
QString gameName( QTreeWidgetItem* item )
{
    return item->text( GAME_NAME_COLUMN );
}
QString gameSize( QTreeWidgetItem* item )
{
    return item->text( GAME_SIZE_COLUMN );
}
QString gameRegion( QTreeWidgetItem* item )
{
    return item->text( GAME_REGION_COLUMN );
}
QString gameType( QTreeWidgetItem* item )
{
    return item->text( GAME_TYPE_COLUMN );
}
QString gamePartitions( QTreeWidgetItem* item )
{
    return item->text( GAME_PARTITIONS_COLUMN );
}
QString gamePath( QTreeWidgetItem* item )
{
    return item->text( GAME_PATH_COLUMN );
}
void SetGameID( QTreeWidgetItem* item, QString s )
{
    item->setText( GAME_ID_COLUMN , s );
}
void SetGameName( QTreeWidgetItem* item, QString s )
{
    item->setText( GAME_NAME_COLUMN , s );
}
void SetGameSize( QTreeWidgetItem* item, QString s )
{
    item->setText( GAME_SIZE_COLUMN , s );
}
void SetGameRegion( QTreeWidgetItem* item, QString s )
{
    item->setText( GAME_REGION_COLUMN , s );
}
void SetGameType( QTreeWidgetItem* item, QString s )
{
    item->setText( GAME_TYPE_COLUMN , s );
}
void SetGamePartitions( QTreeWidgetItem* item, QString s )
{
    item->setText( GAME_PARTITIONS_COLUMN , s );
}
void SetGamePath( QTreeWidgetItem* item, QString s )
{
    item->setText( GAME_PATH_COLUMN , s );
}

char ascii( char s ) {
    if ( s < 0x20 ) return '.';
    if ( s > 0x7E ) return '.';
    return s;
}
void hexdump( void *d, int len ) {
    unsigned char *data;
    int i, off;
    data = (unsigned char*)d;
    printf("\n");
    for ( off = 0; off < len; off += 16 ) {
	printf( "%08x  ", off );
	for ( i=0; i<16; i++ )
	    if ( ( i + off ) >= len ) printf("   ");
	    else printf("%02x ",data[ off + i ]);

	printf(" ");
	for ( i = 0; i < 16; i++ )
	    if ( ( i + off) >= len ) printf(" ");
	    else printf("%c", ascii( data[ off + i ]));
	printf("\n");
    }
    fflush( stdout );
}
