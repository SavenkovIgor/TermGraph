// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/link/linksdecorator.h"

LinksDecorator::LinksDecorator(LinksString                    linksString,
                               LinksDecorator::DecorCondition colorCondition,
                               DecorCondition                 backgroundCondition)
    : mLinksString(linksString)
    , mColorCondition(colorCondition)
    , mBackgroundCondition(backgroundCondition)
{}

QString LinksDecorator::apply(LinksDecoratorMode mode)
{
    auto ret = mLinksString.text();

    for (int i = mLinksString.links().size() - 1; i >= 0; i--) {
        const auto& link = mLinksString.links()[i];

        int rBracketPos = link.right().pos() - 1;
        int lBracketPos = link.left().pos();

        auto color    = mColorCondition(i, link);
        auto back     = mBackgroundCondition(i, link);
        auto colorStr = mLeftReplacer.arg(color.name(QColor::HexArgb)).arg(back.name(QColor::HexArgb));

        if (mode == LinksDecoratorMode::Insert) {
            ret.insert(rBracketPos + 1, mRightReplacer);
            ret.insert(lBracketPos, colorStr);
        } else if (mode == LinksDecoratorMode::Replace) {
            ret.replace(rBracketPos, 1, mRightReplacer);
            ret.replace(lBracketPos, 1, colorStr);
        }
    }

    return ret;
}

QColor LinksDecorator::defaultDecorator([[maybe_unused]] int orderIndex, [[maybe_unused]] const Link& link)
{
    return QColor("#00a693");
}

QColor LinksDecorator::blueDecorator([[maybe_unused]] int orderIndex, [[maybe_unused]] const Link& link)
{
    return QColor("#6d9a28");
}

QColor LinksDecorator::defaultBackground([[maybe_unused]] int orderIndex, [[maybe_unused]] const Link& link)
{
    return QColor("transparent");
}
