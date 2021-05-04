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

#include "source/Model/Termin/termdatacache.h"

#include "source/Helpers/appstyle.h"
#include "source/Helpers/fonts.h"
#include "source/Helpers/helpstuff.h"
#include "source/Helpers/link/linksdecorator.h"
#include "source/Helpers/link/linkutils.h"
#include "source/Helpers/text/textutils.h"

// Initialization order is important!
TermDataCache::TermDataCache(const TermData& info)
    : mTerm(info.term)
    , mDefinition(info.definition)
    , mLowerTerm(info.term.toLower())
    , mDecoratedTerm(getDecoratedTerm(info.term))
    , mDecoratedTermSize(getTermSize(mDecoratedTerm))
    , mLinksDefinition(info.definition)
{}

QString TermDataCache::lowerTerm() const { return mLowerTerm; }

QString TermDataCache::decoratedTerm() const { return mDecoratedTerm; }

QSizeF TermDataCache::decoratedTermSize() const { return mDecoratedTermSize; }

const Link::List& TermDataCache::links() const { return mLinksDefinition.links(); }

QString TermDataCache::termAndDefinition(bool decorated) const
{
    LinksDecorator decorator(mLinksDefinition, LinksDecorator::defaultDecorator);

    if (decorated) {
        auto ret = "<font color=\"#00a693\">" + mTerm + "</font>";
        ret += " - это " + decorator.apply(LinksDecoratorMode::Replace);
        return ret;
    }
    return mTerm + " - это " + mDefinition;
}

QString TermDataCache::getDecoratedTerm(const QString& term)
{
    QString ret = term;

    if (ret.contains(" ")) {
        // Если имя превышает базовую ширину и содержит пробелы то пытаемся его разбить на 2

        if (Fonts::getTextMetrics(ret).width() + 15 > AppStyle::Sizes::baseBlockWidth) {
            // Пытаемся ужать в 2 строки
            ret = TextUtils::insertNewLineNearMiddle(ret);
        }
    }

    return ret;
}

QSizeF TermDataCache::getTermSize(const QString& decoratedTerm)
{
    SizeList sizes;

    auto termParts = decoratedTerm.split("\n");

    for (auto& part : termParts)
        sizes.push_back(Fonts::getTextMetrics(part));

    return HelpStuff::getStackedSize(sizes, Qt::Vertical);
}
