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

#include "source/Helpers/link/linksdecorator.h"

LinksDecorator::LinksDecorator(LinksText                      linksText,
                               LinksDecorator::DecorCondition colorCondition,
                               DecorCondition                 backgroundCondition)
    : mLinksText(linksText)
    , mColorCondition(colorCondition)
    , mBackgroundCondition(backgroundCondition)
{}

QString LinksDecorator::apply(LinksDecoratorMode mode)
{
    auto ret = mLinksText.text();

    for (int i = mLinksText.links().size() - 1; i >= 0; i--) {
        const auto& link = mLinksText.links()[i];

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
