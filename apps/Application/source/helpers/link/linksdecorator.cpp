// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/link/linksdecorator.h"

#include <CommonTools/HandyTypes.h>

LinksDecorator::LinksDecorator(LinksString                    linksString,
                               LinksDecorator::DecorCondition colorCondition,
                               DecorCondition                 backgroundCondition)
    : mLinksString(linksString)
    , mColorCondition(std::move(colorCondition))
    , mBackgroundCondition(std::move(backgroundCondition))
{}

QString LinksDecorator::apply(LinksDecoratorMode mode)
{
    auto ret = mLinksString.text();

    const auto& links = mLinksString.links();

    for (int i = asInt(links.size()) - 1; i >= 0; i--) {
        const auto& link = links[Link::asListSize(i)];

        int rBracketPos = link.right().pos() - 1;
        int lBracketPos = link.left().pos();

        auto color    = mColorCondition(asInt(i), link);
        auto back     = mBackgroundCondition(asInt(i), link);
        auto colorStr = mLeftReplacer.arg(color.name(QColor::HexArgb), back.name(QColor::HexArgb));

        if (mode == LinksDecoratorMode::Insert) {
            ret.insert(rBracketPos + 1, mRightReplacer);
            ret.insert(lBracketPos, colorStr);
        } else if (mode == LinksDecoratorMode::Replace) {
            ret.replace(rBracketPos, 1, mRightReplacer);

            // Remove uuid section
            if (link.hasUuid()) {
                ret.replace(lBracketPos + 1, link.fullLink().length() - 2, link.text().toString());
            }

            ret.replace(lBracketPos, 1, colorStr);
        } else if (mode == LinksDecoratorMode::Cut) {
            ret.remove(rBracketPos, 1);
            // Remove uuid section
            if (link.hasUuid()) {
                ret.replace(lBracketPos + 1, link.fullLink().length() - 2, link.text().toString());
            }
            ret.remove(lBracketPos, 1);
        }
    }

    return ret;
}

QColor LinksDecorator::defaultDecorator([[maybe_unused]] int orderIndex, [[maybe_unused]] const Link& link)
{
    return QColor::fromString("#00a693");
}

QColor LinksDecorator::greenDecorator([[maybe_unused]] int orderIndex, const Link& link)
{
    return link.hasUuid() ? QColor::fromString("#ffcf87") : QColor::fromString("#c1fc9d");
}

QColor LinksDecorator::defaultBackground([[maybe_unused]] int orderIndex, [[maybe_unused]] const Link& link)
{
    return QColor::fromString("transparent");
}
