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

#include "source/Managers/jsongroupinfocontainerparser.h"

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
    ret.insert(TermGroupColumn::name, QJsonValue(info.name));
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
