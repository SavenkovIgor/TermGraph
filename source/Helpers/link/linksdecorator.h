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

#include <functional>

#include <QColor>
#include <QString>

#include "source/Helpers/link/linkstext.h"
#include "source/Helpers/link/textlink.h"

enum LinksDecoratorMode { Replace = 0, Insert };

class LinksDecorator
{
public:
    using DecorCondition = std::function<QColor(int, const TextLink&)>;

    LinksDecorator(LinksText      linksText,
                   DecorCondition colorCondition,
                   DecorCondition backgroundCondition = defaultBackground);

    QString apply(LinksDecoratorMode mode);

    static QColor defaultDecorator(int orderIndex, const TextLink& link);
    static QColor blueDecorator(int orderIndex, const TextLink& link);

    static QColor defaultBackground(int orderIndex, const TextLink& link);

private: // Members
    const LinksText      mLinksText;
    const DecorCondition mColorCondition;
    const DecorCondition mBackgroundCondition;

    const QString mLeftReplacer  = QStringLiteral("<font color=\"%1\" style=\"background-color:%2\">");
    const QString mRightReplacer = QStringLiteral("</font>");
};
