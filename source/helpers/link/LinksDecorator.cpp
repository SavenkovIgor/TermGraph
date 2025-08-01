// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <functional>

#include <QColor>
#include <QString>

export module Link.LinksDecorator;

import CommonTools.HandyTypes;
import Link.Link;
import Link.LinksString;

using namespace Qt::Literals::StringLiterals;

export enum LinksDecoratorMode { Replace = 0, Insert, Cut };

// TODO: Rework this class. After a couple of reworks it became messy and unefficient
export class LinksDecorator
{
public:
    using DecorCondition = std::function<QColor(int, const Link&)>;

    LinksDecorator(LinksString    linksString,
                   DecorCondition colorCondition,
                   DecorCondition backgroundCondition = defaultBackground)
        : mLinksString(linksString)
        , mColorCondition(std::move(colorCondition))
        , mBackgroundCondition(std::move(backgroundCondition))
    {}

    QString apply(LinksDecoratorMode mode)
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

    static QColor greenDecorator([[maybe_unused]] int orderIndex, const Link& link)
    {
        return link.hasUuid() ? QColor::fromString("#ffcf87") : QColor::fromString("#c1fc9d");
    }

    static QColor defaultBackground([[maybe_unused]] int orderIndex, [[maybe_unused]] const Link& link)
    {
        return QColor::fromString("transparent");
    }

    static QString toRichText(const LinksString& linksString)
    {
        LinksDecorator decorator(linksString, LinksDecorator::greenDecorator);
        return decorator.apply(LinksDecoratorMode::Replace);
    }

    static QString toPlainString(const LinksString& linksString)
    {
        LinksDecorator decorator(linksString, LinksDecorator::greenDecorator);
        return decorator.apply(LinksDecoratorMode::Cut);
    }

private: // Members
    const LinksString    mLinksString;
    const DecorCondition mColorCondition;
    const DecorCondition mBackgroundCondition;

    const QString mLeftReplacer  = u"<font color=\"%1\" style=\"background-color:%2\">"_s;
    const QString mRightReplacer = u"</font>"_s;
};
