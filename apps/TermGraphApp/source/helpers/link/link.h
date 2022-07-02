// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QString>
#include <QStringView>
#include <QUuid>

#include <CommonTools/HandyTypes.h>

#include "source/helpers/text/textrange.h"

class Link : public TextRange
{
public:
    using List = std::vector<Link>;

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
