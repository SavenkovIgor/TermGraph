#ifndef JSONNODEINFOCONTAINERPARSER_H
#define JSONNODEINFOCONTAINERPARSER_H

#include <QJsonObject>

#include "../Model/Termin/nodeinfocontainer.h"

class JsonNodeInfoContainerParser
{
public:
    static NodeInfoContainer fromJson(const QJsonObject& jsonObj);
    static QJsonObject toJson(const NodeInfoContainer& info);
};

#endif // JSONNODEINFOCONTAINERPARSER_H
