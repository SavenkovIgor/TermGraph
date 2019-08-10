#include "jsonnodeinfocontainerparser.h"

#include "source/databaseWorks/columns/nodecolumn.h"

NodeInfoContainer JsonNodeInfoContainerParser::fromJson(const QJsonObject& jsonObj)
{
    NodeInfoContainer info;

    info.uuid = QUuid(jsonObj[NodeColumn::uuid].toString());

    if (jsonObj.contains(NodeColumn::term))
        info.term = jsonObj[NodeColumn::term].toString();

    info.definition  = jsonObj[NodeColumn::definition].toString();
    info.description = jsonObj[NodeColumn::description].toString();
    info.examples    = jsonObj[NodeColumn::examples].toString();
    info.wikiUrl     = jsonObj[NodeColumn::wikiUrl].toString();
    info.wikiImage   = jsonObj[NodeColumn::wikiImage].toString();
    info.groupUuid   = QUuid(jsonObj[NodeColumn::groupUuid].toString());

    if (jsonObj.contains(NodeColumn::lastEdit)) {
        info.lastEdit = QDateTime::fromString(jsonObj[NodeColumn::lastEdit].toString(), Qt::ISODate);
    }

    return info;
}

QJsonObject JsonNodeInfoContainerParser::toJson(const NodeInfoContainer &info)
{
    QJsonObject ret;

    ret.insert(NodeColumn::uuid,        QJsonValue(info.uuid.toString()));
    ret.insert(NodeColumn::term,        QJsonValue(info.term));
    ret.insert(NodeColumn::definition,  QJsonValue(info.definition));
    ret.insert(NodeColumn::description, QJsonValue(info.description));
    ret.insert(NodeColumn::examples,    QJsonValue(info.examples));
    ret.insert(NodeColumn::wikiUrl,     QJsonValue(info.wikiUrl));
    ret.insert(NodeColumn::wikiImage,   QJsonValue(info.wikiImage));
    ret.insert(NodeColumn::groupUuid,   QJsonValue(info.groupUuid.toString()));
    ret.insert(NodeColumn::lastEdit,    QJsonValue(info.lastEdit.toString(Qt::ISODate)));

    return ret;
}
