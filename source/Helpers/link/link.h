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

#pragma once

#include <QString>
#include <QStringView>
#include <QUuid>

#include "source/Helpers/handytypes.h"
#include "source/Helpers/text/textrange.h"

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

    static opt<Link> select(QStringView str, int startPos);

private: //Methods
    static QStringView getText(QStringView fullLink);
    static QString     getLower(QStringView text);

    static opt<QUuid> tryGetUuid(QStringView fullLink);

private: // Members
    const QStringView mLinkText;
    const QString     mLinkTextLower;
    const Type        mLinkType = Type::Unknown;
    const QUuid       mUuid     = QUuid();
};
