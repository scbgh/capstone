//
// mapfile.cpp
//

#include <string>
#include <map>
#include "mapfile.h"
#include "json/picojson.h"

namespace j = picojson;

//

//
//
QString mapToJson(QSharedPointer<GameMap> map)
{
    std::map<std::string, j::value> rootArray;
    rootArray["width"] = j::value(map->width);
    rootArray["height"] = j::value(map->height);

    return QString::fromStdString(j::value(rootArray).serialize());
}

//
//
QSharedPointer<GameMap> jsonToMap(const QString& json)
{
}