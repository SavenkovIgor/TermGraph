/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

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
