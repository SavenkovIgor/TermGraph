#pragma once

#include <QJsonObject>

#include "source/Model/Termin/nodeinfocontainer.h"

class JsonNodeInfoContainerParser
{
public:
    static NodeInfoContainer fromJson(const QJsonObject& jsonObj);
    static QJsonObject toJson(const NodeInfoContainer& info);
};
