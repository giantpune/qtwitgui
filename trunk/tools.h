#ifndef TOOLS_H
#define TOOLS_H

#include "includes.h"

#define SETTINGS_VERSION 2

#define _KiB_ ( (float)0x40000 )
#define _MiB_ ( (float)0x100000 )
#define _GiB_ ( (float)0x40000000 )

#define MIN( x, y ) ( ( x ) < ( y ) ? ( x ) : ( y ) )
#define MAX( x, y ) ( ( x ) > ( y ) ? ( x ) : ( y ) )

#define RU(x,n)	(-(-(x) & -(n)))    //round up
#define ALIGN 0x8000		    //alignment for GC files

extern QString settingsPath;

QString SizeTextGiB( QString bytes );

#define GAME_ID_COLUMN		    0
#define GAME_NAME_COLUMN	    1
#define GAME_SIZE_COLUMN	    2
#define GAME_REGION_COLUMN	    3
#define GAME_TYPE_COLUMN	    4
#define GAME_PARTITIONS_COLUMN	    5
#define GAME_PATH_COLUMN	    6

QString gameID( QTreeWidgetItem* item );
QString gameName( QTreeWidgetItem* item );
QString gameSize( QTreeWidgetItem* item );
QString gameRegion( QTreeWidgetItem* item );
QString gameType( QTreeWidgetItem* item );
QString gamePartitions( QTreeWidgetItem* item );
QString gamePath( QTreeWidgetItem* item );

void SetGameID( QTreeWidgetItem*, QString );
void SetGameName( QTreeWidgetItem* item, QString s );
void SetGameSize( QTreeWidgetItem* item, QString s );
void SetGameRegion( QTreeWidgetItem* item, QString s );
void SetGameType( QTreeWidgetItem* item, QString s );
void SetGamePartitions( QTreeWidgetItem* item, QString s );
void SetGamePath( QTreeWidgetItem* item, QString s );
#endif // TOOLS_H
