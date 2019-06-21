#ifndef JSONINFOCONTAINERPARSER_H
#define JSONINFOCONTAINERPARSER_H

#include <QJsonObject>

#include "../Model/Termin/nodeinfocontainer.h"

class JsonInfoContainerParser
{
public:
    static NodeInfoContainer fromJson(const QJsonObject& jsonObj);
    static QJsonObject toJson(const NodeInfoContainer& info);
};

#endif // JSONINFOCONTAINERPARSER_H
