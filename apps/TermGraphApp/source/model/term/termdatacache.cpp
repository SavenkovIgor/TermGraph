// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/model/term/termdatacache.h"

#include "source/helpers/link/linksdecorator.h"
#include "source/helpers/text/texttools.h"


// Initialization order is important!
TermDataCache::TermDataCache(const TermData& info)
    : mTerm(info.term)
    , mDefinition(info.definition)
    , mLowerTerm(info.term.toLower())
    , mTermSize(TextTools::preferredTextSize(info.term))
    , mLinksDefinition(info.definition)
{}

QString TermDataCache::term() const { return mTerm; }

QString TermDataCache::lowerTerm() const { return mLowerTerm; }

QSizeF TermDataCache::preferredSize() const { return mTermSize; }

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
