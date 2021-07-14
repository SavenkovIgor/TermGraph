/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
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

#include "source/managers/jsontermdataparser.h"

bool JsonTermDataParser::isValidKeysAndTypes(const QJsonObject& json)
{
    using namespace std;

    auto hasKey = [&json](const auto& key) -> bool { return json.contains(key) && json[key].isString(); };

    QStringList keys
        = {"uuid", "term", "definition", "description", "examples", "wikiUrl", "wikiImage", "groupUuid", "lastEdit"};

    return all_of(begin(keys), end(keys), hasKey);
}

bool JsonTermDataParser::isValid(const QJsonObject& json, bool checkUuid, bool checkGroupUuid, bool checkLastEdit)
{
    if (!isValidKeysAndTypes(json))
        return false;

    if (json["term"].toString().isEmpty())
        return false;

    if (checkUuid) {
        if (QUuid(json["uuid"].toString()).isNull())
            return false;
    }

    if (checkGroupUuid) {
        if (QUuid(json["groupUuid"].toString()).isNull())
            return false;
    }

    if (checkLastEdit) {
        auto lastEdit = QDateTime::fromString(json["lastEdit"].toString(), Qt::ISODate);
        if (lastEdit.isNull())
            return false;
    }

    return true;
}

TermData JsonTermDataParser::fromJson(const QJsonObject& jsonObj)
{
    TermData info;

    info.uuid = QUuid(jsonObj["uuid"].toString());

    if (jsonObj.contains("term"))
        info.term = jsonObj["term"].toString();

    info.definition  = jsonObj["definition"].toString();
    info.description = jsonObj["description"].toString();
    info.examples    = jsonObj["examples"].toString();
    info.wikiUrl     = jsonObj["wikiUrl"].toString();
    info.wikiImage   = jsonObj["wikiImage"].toString();
    info.groupUuid   = QUuid(jsonObj["groupUuid"].toString());

    if (jsonObj.contains("lastEdit")) {
        info.lastEdit = QDateTime::fromString(jsonObj["lastEdit"].toString(), Qt::ISODate);
    }

    return info;
}

QJsonObject JsonTermDataParser::toJson(const TermData& info)
{
    QJsonObject ret;

    ret.insert("uuid", QJsonValue(info.uuid.toString()));
    ret.insert("term", QJsonValue(info.term));
    ret.insert("definition", QJsonValue(info.definition));
    ret.insert("description", QJsonValue(info.description));
    ret.insert("examples", QJsonValue(info.examples));
    ret.insert("wikiUrl", QJsonValue(info.wikiUrl));
    ret.insert("wikiImage", QJsonValue(info.wikiImage));
    ret.insert("groupUuid", QJsonValue(info.groupUuid.toString()));
    ret.insert("lastEdit", QJsonValue(info.lastEdit.toString(Qt::ISODate)));

    return ret;
}