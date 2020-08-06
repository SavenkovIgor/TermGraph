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

#include "source/Model/Termin/innerlink.h"

#include "source/Helpers/linkutils.h"
#include "source/Helpers/text/chartools.h"

InnerLink::InnerLink(const QString& srcString, TextRange range)
    : mBaseString(srcString)
    , mLink(getLink(srcString, range))
    , mLinkText(getText(mLink))
    , mLinkTextLower(getLower(mLinkText))
{
    auto optUuid = tryGetUuid(mLink);
    mLinkType    = optUuid.has_value() ? Type::Uuid : Type::Text;
    mUuid        = optUuid.value_or(QUuid());
}

QStringView InnerLink::fullLink() const { return mLink; }

QStringView InnerLink::text() const { return mLinkText; }

const QString& InnerLink::textLower() const { return mLinkTextLower; }

bool InnerLink::hasUuid() const { return mLinkType == Type::Uuid; }

QUuid InnerLink::uuid() const
{
    assert(hasUuid());
    return mUuid;
}

QStringView InnerLink::getLink(const QString& srcString, TextRange range)
{
    assert(srcString.size() > 2);
    assert(LinkUtils::isRangeOnLink(srcString, range));

    assert(range.size() >= 2);

    return QStringView(srcString.midRef(range.leftPos(), range.size()));
}

QStringView InnerLink::getText(QStringView fullLink)
{
    const auto from     = 1;
    const auto splitter = fullLink.lastIndexOf('|');
    const auto to       = splitter == -1 ? fullLink.size() - 2 : splitter - 1;
    const auto size     = to - from + 1;

    return QStringView(fullLink.mid(1, size));
}

QString InnerLink::getLower(QStringView text) { return text.toString().toLower(); }

opt<QUuid> InnerLink::tryGetUuid(QStringView fullLink)
{
    assert(fullLink[fullLink.size() - 1] == CharTools::rightBracket);

    auto index = fullLink.lastIndexOf('|');
    if (index != -1) {
        auto size    = fullLink.size() - 1 - index;
        auto uuidCut = fullLink.mid(index + 1, size).toString(); // Without braces
        auto uuidStr = CharTools::leftBracket + uuidCut + CharTools::rightBracket;
        auto uuid    = QUuid::fromString(uuidStr);
        if (!uuid.isNull())
            return uuid;
    }

    return std::nullopt;
}
