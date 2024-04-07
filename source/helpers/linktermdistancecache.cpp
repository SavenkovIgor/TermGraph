// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/linktermdistancecache.h"

std::optional<std::optional<int>> LinkTermDistanceCache::get(const QString& link, const QString& term) const
{
    auto linkIter = linkToTermCache.constFind(link);
    if (linkIter != linkToTermCache.constEnd()) {
        auto termIter = linkIter.value().constFind(term);
        if (termIter != linkIter.value().constEnd()) {
            return std::optional<std::optional<int>>(termIter.value());
        }
    }

    return std::nullopt;
}

void LinkTermDistanceCache::add(const QString& link, const QString& term, std::optional<int> value)
{
    linkToTermCache[link][term] = value;
}
