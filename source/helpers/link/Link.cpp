// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QString>
#include <QStringView>
#include <QUuid>

export module Link.Link;

import Text;

export class Link : public TextRange
{
public:
    // TODO: Remove later. Sign of bad typing approach
    static inline auto asListSize = [](auto num) -> std::vector<Link>::size_type { return static_cast<std::vector<Link>::size_type>(num); };

    enum class Type { Unknown = 0, Text, Uuid };

    Link(QStringView strView, int left, int right)
        : TextRange(strView, left, right)
        , mLinkText(getText(rangeView()))
        , mLinkTextLower(getLower(mLinkText))
        , mLinkType(tryGetUuid(rangeView()).has_value() ? Type::Uuid : Type::Text)
        , mUuid(tryGetUuid(rangeView()).value_or(QUuid()))
    {
        assert(this->left().right() == CharTools::leftBracket);
        assert(this->right().left() == CharTools::rightBracket);
    }

    QStringView    fullLink() const { return rangeView(); }
    QStringView    text() const { return mLinkText; }
    const QString& textLower() const { return mLinkTextLower; }
    bool           hasUuid() const { return mLinkType == Type::Uuid; }
    QUuid          uuid() const
    {
        assert(hasUuid());
        return mUuid;
    }

    [[nodiscard]] QString createLinkWithUuid(const QUuid& uuid) const
    {
        auto linkText = mLinkText.toString();
        linkText += CharTools::linkSplitter + uuid.toString(QUuid::WithoutBraces);
        linkText = CharTools::leftBracket + linkText + CharTools::rightBracket;
        return linkText;
    }

    static std::optional<Link> select(QStringView str, int startPos)
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

    static bool isCursorOnLink(QStringView str, int cursorPos) { return select(str, cursorPos).has_value(); }

private: //Methods
    static QStringView getText(QStringView fullLink)
    {
        const auto from     = 1;
        const auto splitter = fullLink.lastIndexOf('|');
        const auto to       = splitter == -1 ? fullLink.size() - 2 : splitter - 1;
        const auto size     = to - from + 1;

        return QStringView(fullLink.mid(1, size));
    }

    static QString getLower(QStringView text) { return text.toString().toLower(); }

    static std::optional<QUuid> tryGetUuid(QStringView fullLink)
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

private: // Members
    const QStringView mLinkText;
    const QString     mLinkTextLower;
    const Type        mLinkType = Type::Unknown;
    const QUuid       mUuid     = QUuid();
};
