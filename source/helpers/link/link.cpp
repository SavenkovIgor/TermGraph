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

#include "source/helpers/link/link.h"

#include "source/helpers/text/chartools.h"
#include "source/helpers/text/checkingtextcursor.h"
#include "source/helpers/uuid/uuidtools.h"

Link::Link(QStringView strView, int left, int right)
    : TextRange(strView, left, right)
    , mLinkText(getText(rangeView()))
    , mLinkTextLower(getLower(mLinkText))
    , mLinkType(tryGetUuid(rangeView()).has_value() ? Type::Uuid : Type::Text)
    , mUuid(tryGetUuid(rangeView()).value_or(QUuid()))
{
    assert(this->left().right() == CharTools::leftBracket);
    assert(this->right().left() == CharTools::rightBracket);
}

QStringView Link::fullLink() const { return rangeView(); }

QStringView Link::text() const { return mLinkText; }

const QString& Link::textLower() const { return mLinkTextLower; }

bool Link::hasUuid() const { return mLinkType == Type::Uuid; }

QUuid Link::uuid() const
{
    assert(hasUuid());
    return mUuid;
}

QString Link::createLinkWithUuid(const QUuid& uuid) const
{
    auto linkText = mLinkText.toString();
    linkText += CharTools::linkSplitter + UuidTools::cutBraces(uuid);
    linkText = CharTools::leftBracket + linkText + CharTools::rightBracket;
    return linkText;
}

opt<Link> Link::select(QStringView str, int startPos)
{
    if (!TextCursor::isValidCursor(str, startPos))
        return std::nullopt;

    auto cursor = TextCursor(str, startPos);

    auto lBracket = CheckingTextCursor::anyBracketOnLeft(str, startPos, Direction::Left);
    auto rBracket = CheckingTextCursor::anyBracketOnRight(str, startPos, Direction::Right);

    if (!lBracket.check() || !rBracket.check())
        return std::nullopt;

    lBracket--;
    rBracket++;

    if (lBracket.right() == CharTools::leftBracket && rBracket.left() == CharTools::rightBracket)
        return Link(str, lBracket.pos(), rBracket.pos());

    return std::nullopt;
}

bool Link::isCursorOnLink(QStringView str, int cursorPos) { return select(str, cursorPos).has_value(); }

QStringView Link::getText(QStringView fullLink)
{
    const auto from     = 1;
    const auto splitter = fullLink.lastIndexOf('|');
    const auto to       = splitter == -1 ? fullLink.size() - 2 : splitter - 1;
    const auto size     = to - from + 1;

    return QStringView(fullLink.mid(1, size));
}

QString Link::getLower(QStringView text) { return text.toString().toLower(); }

opt<QUuid> Link::tryGetUuid(QStringView fullLink)
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
