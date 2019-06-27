#include "jsongroupinfocontainerparser.h"

#include "../databaseWorks/columns/termgroupcolumn.h"

GroupInfoContainer JsonGroupInfoContainerParser::fromJson(const QJsonObject &jsonObj)
{
    GroupInfoContainer info;

    info.uuid    = QUuid(jsonObj[TermGroupColumn::longUID].toString());
    info.name    = jsonObj[TermGroupColumn::name].toString();
    info.comment = jsonObj[TermGroupColumn::comment].toString();
    info.type    = static_cast<GroupType>(jsonObj[TermGroupColumn::type].toInt());

    return info;
}

QJsonObject JsonGroupInfoContainerParser::toJson(const GroupInfoContainer &info)
{
    QJsonObject ret;

    ret.insert(TermGroupColumn::longUID, QJsonValue(info.uuid.toString()));
    ret.insert(TermGroupColumn::name,    QJsonValue(info.name));
    ret.insert(TermGroupColumn::comment, QJsonValue(info.comment));
    ret.insert(TermGroupColumn::type,    QJsonValue(static_cast<int>(info.type)));

    return ret;
}
