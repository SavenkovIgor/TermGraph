// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/model/term/termdatacache.h"

#include "source/helpers/appstyle.h"
#include "source/helpers/fonts.h"
#include "source/helpers/link/linksdecorator.h"
#include "source/helpers/text/texttools.h"


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

        if (Fonts::metrics(ret).width() + 15 > AppStyle::Sizes::baseBlockWidth) {
            // Пытаемся ужать в 2 строки
            ret = TextTools::insertNewLineNearMiddle(ret);
        }
    }

    return ret;
}

QSizeF TermDataCache::getTermSize(const QString& decoratedTerm)
{
    SizeList sizes;

    auto termParts = decoratedTerm.split("\n");

    for (auto& part : termParts)
        sizes.push_back(Fonts::metrics(part));

    return sizes.totalStackedSize(Qt::Vertical);
}
