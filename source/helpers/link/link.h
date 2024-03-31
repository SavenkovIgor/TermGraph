// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QString>
#include <QStringView>
#include <QUuid>

#include <source/CommonTools/HandyTypes.h>

#include <source/Text/TextRange.h>

class Link : public TextRange
{
public:
    using List                    = std::vector<Link>;
    static inline auto asListSize = [](auto num) -> List::size_type { return static_cast<List::size_type>(num); };

    enum class Type { Unknown = 0, Text, Uuid };

    Link(QStringView strView, int left, int right);

    QStringView    fullLink() const;
    QStringView    text() const;
    const QString& textLower() const;
    bool           hasUuid() const;
    QUuid          uuid() const;

    [[nodiscard]] QString createLinkWithUuid(const QUuid& uuid) const;

    static Opt<Link> select(QStringView str, int startPos);

    static bool isCursorOnLink(QStringView str, int cursorPos);

private: //Methods
    static QStringView getText(QStringView fullLink);
    static QString     getLower(QStringView text);

    static Opt<QUuid> tryGetUuid(QStringView fullLink);

private: // Members
    const QStringView mLinkText;
    const QString     mLinkTextLower;
    const Type        mLinkType = Type::Unknown;
    const QUuid       mUuid     = QUuid();
};
