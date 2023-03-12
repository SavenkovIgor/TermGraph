// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/model/term/termdatacache.h"

#include <Text/TextTools.h>


// Initialization order is important!
TermDataCache::TermDataCache(const TermData& info)
    : mTerms{info.term}
    , mLowerTerms{info.term.toLower()}
    , mTermSize(TextTools::preferredTextSize(info.term))
    , mLinksDefinition(info.definition)
{
    assert(!mTerms.empty());
}

void TermDataCache::addSynonym(const QString& synonym)
{
    mTerms.push_back(synonym);
    mLowerTerms.push_back(synonym.toLower());
}

bool TermDataCache::isSynonym() const {
    if (mLinksDefinition.links().size() != 1) {
        return false;
    }

    auto withoutLinks = mLinksDefinition.replaceLink(0, "");
    return withoutLinks.simplified().isEmpty();
}

bool TermDataCache::hasSynonyms() const { return mTerms.size() > 1; }

QString TermDataCache::definition() const { return mLinksDefinition.text(); }

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
        auto ret = "<font color=\"#00a693\">" + term() + "</font>";
        ret += " - " + mLinksDefinition.toRichText();
        return ret;
    }
    return term() + " - " + mLinksDefinition.text();
}
