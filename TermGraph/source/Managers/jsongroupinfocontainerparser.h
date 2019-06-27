#ifndef JSONGROUPINFOCONTAINERPARSER_H
#define JSONGROUPINFOCONTAINERPARSER_H

#include <QJsonObject>

#include "../Model/TerminGroup/groupinfocontainer.h"

class JsonGroupInfoContainerParser
{
public:
    static GroupInfoContainer fromJson(const QJsonObject& jsonObj);
    static QJsonObject toJson(const GroupInfoContainer& info);
};

#endif // JSONGROUPINFOCONTAINERPARSER_H
