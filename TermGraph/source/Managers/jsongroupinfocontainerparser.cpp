#include "jsongroupinfocontainerparser.h"

#include "source/databaseWorks/columns/termgroupcolumn.h"

GroupInfoContainer JsonGroupInfoContainerParser::fromJson(const QJsonObject& jsonObj)
{
    GroupInfoContainer info;

    info.uuid    = QUuid(jsonObj[TermGroupColumn::uuid].toString());
    info.name    = jsonObj[TermGroupColumn::name].toString();
    info.comment = jsonObj[TermGroupColumn::comment].toString();

    return info;
}

QJsonObject JsonGroupInfoContainerParser::toJson(const GroupInfoContainer& info)
{
    QJsonObject ret;

    ret.insert(TermGroupColumn::uuid, QJsonValue(info.uuid.toString()));
    ret.insert(TermGroupColumn::name,    QJsonValue(info.name));
    ret.insert(TermGroupColumn::comment, QJsonValue(info.comment));

    return ret;
}

/*
 * bool TermGroup::checkJson(QJsonDocument doc)  // TODO: Вынести это отсюда!
{
    QJsonObject json = doc.object();

QStringList checkKeys;
checkKeys << "longUID";
checkKeys << "name";
checkKeys << "type";
checkKeys << "nodesList";

for (auto key : checkKeys) {
    if (!json.contains(key)) {
        return false;
    }
}

return true;
}
 */
