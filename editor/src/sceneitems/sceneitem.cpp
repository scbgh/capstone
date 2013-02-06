//
// sceneitem.cpp
//

#include "sceneitems/connectitem.h"
#include "sceneitem.h"

void SceneItem::syncConnections()
{
    for (auto& c : connections_) {
        c->sync();
    }
}