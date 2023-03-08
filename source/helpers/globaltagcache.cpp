// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/globaltagcache.h"


Opt<Opt<int>> GlobalTagCache::get(const QString& tag, const QString& term) const
{
    auto tagIter = tagToTermCache.constFind(tag);
    if (tagIter != tagToTermCache.constEnd()) {
        auto termIter = tagIter.value().constFind(term);
        if (termIter != tagIter.value().constEnd()) {
            return Opt<Opt<int>>(termIter.value());
        }
    }

    return std::nullopt;
}

void GlobalTagCache::add(const QString& tag, const QString& term, Opt<int> value) { tagToTermCache[tag][term] = value; }
