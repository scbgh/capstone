//
// mapfile.h
// Handles saving and loading of maps to and from disk
//

#ifndef _MAPFILE_H_
#define _MAPFILE_H_

#include <QtCore>
#include "mapdata.h"

QString mapToJson(QSharedPointer<GameMap> map);
QSharedPointer<GameMap> jsonToMap(const QString& json);

#endif
