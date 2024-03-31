// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/link/link.h"

#include <source/Text/CharTools.h>
#include <source/Text/CheckingTextCursor.h>

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
    linkText += CharTools::linkSplitter + uuid.toString(QUuid::WithoutBraces);
    linkText = CharTools::leftBracket + linkText + CharTools::rightBracket;
    return linkText;
}

Opt<Link> Link::select(QStringView str, int startPos)
{
    if (!TextCursor::isValidCursor(str, startPos)) {
        return std::nullopt;
    }

    auto lBracket = CheckingTextCursor::anyBracketOnLeft(str, startPos, Direction::Left);
    auto rBracket = CheckingTextCursor::anyBracketOnRight(str, startPos, Direction::Right);

    if (!lBracket.check() || !rBracket.check()) {
        return std::nullopt;
    }

    lBracket--;
    rBracket++;

    if (lBracket.right() == CharTools::leftBracket && rBracket.left() == CharTools::rightBracket) {
        return Link(str, lBracket.pos(), rBracket.pos());
    }

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

Opt<QUuid> Link::tryGetUuid(QStringView fullLink)
{
    assert(fullLink[fullLink.size() - 1] == CharTools::rightBracket);

    auto index = fullLink.lastIndexOf('|');
    if (index != -1) {
        auto size    = fullLink.size() - 1 - index;
        auto uuidCut = fullLink.mid(index + 1, size).toString(); // Without braces
        auto uuidStr = CharTools::leftBracket + uuidCut + CharTools::rightBracket;
        auto uuid    = QUuid::fromString(uuidStr);
        if (!uuid.isNull()) {
            return uuid;
        }
    }

    return std::nullopt;
}
