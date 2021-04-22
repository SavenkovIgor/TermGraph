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

#include "source/Helpers/uuid/uuidtools.h"

#include <QRegularExpression>

#include "source/Helpers/text/chartools.h"
#include "source/Helpers/uuid/uuidtools.h"

opt<QUuid> UuidTools::createFromStringWithoutBraces(const QString &uuidString)
{
    if (!UuidTools::isValidUuidStringWithoutBraces(uuidString))
        return std::nullopt;

    auto innerUuidString = uuidString;
    innerUuidString.prepend(CharTools::leftBracket);
    innerUuidString.append(CharTools::rightBracket);

    auto uuid = QUuid::fromString(innerUuidString);

    if (uuid.isNull())
        return std::nullopt;

    return uuid;
}

QString UuidTools::cutBraces(const QUuid &uuid)
{
    auto uuidStr = uuid.toString();

    assert(!uuidStr.isEmpty());
    assert(uuidStr[0] == CharTools::leftBracket);
    assert(uuidStr[uuidStr.length() - 1] == CharTools::rightBracket);

    uuidStr.chop(1);
    uuidStr.remove(0, 1);

    return uuidStr;
}

bool UuidTools::isValidUuidString(const QString &uuidString)
{
    QRegularExpression check(validUuidRegExp);
    return check.match(uuidString).hasMatch();
}

bool UuidTools::isValidUuidStringWithoutBraces(const QString &uuidString)
{
    QRegularExpression check(validUuidWithoutBracesRegExp);
    return check.match(uuidString).hasMatch();
}
