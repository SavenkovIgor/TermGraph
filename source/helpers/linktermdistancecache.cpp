// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/linktermdistancecache.h"


Opt<Opt<int>> LinkTermDistanceCache::get(const QString& link, const QString& term) const
{
    auto linkIter = linkToTermCache.constFind(link);
    if (linkIter != linkToTermCache.constEnd()) {
        auto termIter = linkIter.value().constFind(term);
        if (termIter != linkIter.value().constEnd()) {
            return Opt<Opt<int>>(termIter.value());
        }
    }

    return std::nullopt;
}

void LinkTermDistanceCache::add(const QString& link, const QString& term, Opt<int> value) { linkToTermCache[link][term] = value; }
