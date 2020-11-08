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

#include "source/Helpers/text/textlink.h"

#include "source/Helpers/text/chartools.h"
#include "source/Helpers/uuid/uuidtools.h"

TextLink::TextLink(QStringView strView, int left, int right)
    : TextRange(strView, left, right)
    , mLinkText(getText(rangeView()))
    , mLinkTextLower(getLower(mLinkText))
    , mLinkType(tryGetUuid(rangeView()).has_value() ? Type::Uuid : Type::Text)
    , mUuid(tryGetUuid(rangeView()).value_or(QUuid()))
{
    assert(this->left().right() == CharTools::leftBracket);
    assert(this->right().left() == CharTools::rightBracket);
}

QStringView TextLink::fullLink() const { return rangeView(); }

QStringView TextLink::text() const { return mLinkText; }

const QString& TextLink::textLower() const { return mLinkTextLower; }

bool TextLink::hasUuid() const { return mLinkType == Type::Uuid; }

QUuid TextLink::uuid() const
{
    assert(hasUuid());
    return mUuid;
}

QString TextLink::createLinkWithUuid(const QUuid& uuid) const
{
    auto linkText = mLinkText.toString();
    linkText += CharTools::linkSplitter + UuidTools::cutBraces(uuid);
    linkText = CharTools::leftBracket + linkText + CharTools::rightBracket;
    return linkText;
}

QStringView TextLink::getText(QStringView fullLink)
{
    const auto from     = 1;
    const auto splitter = fullLink.lastIndexOf('|');
    const auto to       = splitter == -1 ? fullLink.size() - 2 : splitter - 1;
    const auto size     = to - from + 1;

    return QStringView(fullLink.mid(1, size));
}

QString TextLink::getLower(QStringView text) { return text.toString().toLower(); }

opt<QUuid> TextLink::tryGetUuid(QStringView fullLink)
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
