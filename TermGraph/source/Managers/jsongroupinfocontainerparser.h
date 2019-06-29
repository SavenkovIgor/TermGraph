#pragma once

#include <QJsonObject>

#include "source/Model/TerminGroup/groupinfocontainer.h"

class JsonGroupInfoContainerParser
{
public:
    static GroupInfoContainer fromJson(const QJsonObject& jsonObj);
    static QJsonObject toJson(const GroupInfoContainer& info);
};
