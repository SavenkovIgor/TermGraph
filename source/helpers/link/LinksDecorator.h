// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <functional>

#include <QColor>
#include <QString>

#include "source/helpers/link/Link.h"
#include "source/helpers/link/LinksString.h"

using namespace Qt::Literals::StringLiterals;

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

    const QString mLeftReplacer  = u"<font color=\"%1\" style=\"background-color:%2\">"_s;
    const QString mRightReplacer = u"</font>"_s;
};
