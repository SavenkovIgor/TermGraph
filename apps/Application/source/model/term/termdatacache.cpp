// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/model/term/termdatacache.h"

#include <Text/TextTools.h>


// Initialization order is important!
TermDataCache::TermDataCache(const TermData& info)
    : mTerm(info.term)
    , mLowerTerm(info.term.toLower())
    , mTermSize(TextTools::preferredTextSize(info.term))
    , mLinksDefinition(info.definition)
{}

QString TermDataCache::term() const { return mTerm; }

QString TermDataCache::definition() const { return mLinksDefinition.text(); }

QString TermDataCache::lowerTerm() const { return mLowerTerm; }

QSizeF TermDataCache::preferredSize() const {
    if (mLinksDefinition.text().isEmpty()) {
        return mTermSize;
    }

    auto defWithoutLinks = mLinksDefinition.toPlainString();
    auto defSize = TextTools::preferredTextSize(defWithoutLinks, 5);

    auto width = std::max(mTermSize.width(), defSize.width());
    auto height = mTermSize.height() + defSize.height();

    return {width, height};
}

const Link::List& TermDataCache::links() const { return mLinksDefinition.links(); }

QString TermDataCache::termAndDefinition(bool decorated) const
{
    if (decorated) {
        auto ret = "<font color=\"#00a693\">" + mTerm + "</font>";
        ret += " - это " + mLinksDefinition.toRichText();
        return ret;
    }
    return mTerm + " - это " + mLinksDefinition.text();
}
