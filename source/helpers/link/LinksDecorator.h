// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <functional>

#include <QColor>
#include <QString>

#include "source/helpers/link/Link.h"
#include "source/helpers/link/LinksString.h"

enum LinksDecoratorMode { Replace = 0, Insert, Cut };

class LinksDecorator
{
public:
    using DecorCondition = std::function<QColor(int, const Link&)>;

    LinksDecorator(LinksString    linksString,
                   DecorCondition colorCondition,
                   DecorCondition backgroundCondition = defaultBackground);

    QString apply(LinksDecoratorMode mode);

    static QColor greenDecorator(int orderIndex, const Link& link);

    static QColor defaultBackground(int orderIndex, const Link& link);

private: // Members
    const LinksString    mLinksString;
    const DecorCondition mColorCondition;
    const DecorCondition mBackgroundCondition;

    const QString mLeftReplacer  = QStringLiteral("<font color=\"%1\" style=\"background-color:%2\">");
    const QString mRightReplacer = QStringLiteral("</font>");
};
