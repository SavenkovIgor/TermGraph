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

#include "source/helpers/globaltagcache.h"

GlobalTagCache& GlobalTagCache::instance()
{
    static GlobalTagCache mgr;
    return mgr;
}

opt<opt<int>> GlobalTagCache::get(const QString& tag, const QString& term)
{
    auto tagIter = tagToTermCache.constFind(tag);
    if (tagIter != tagToTermCache.constEnd()) {
        auto termIter = tagIter.value().constFind(term);
        if (termIter != tagIter.value().constEnd())
            return opt<opt<int>>(termIter.value());
    }

    return std::nullopt;
}

void GlobalTagCache::add(const QString& tag, const QString& term, opt<int> value) { tagToTermCache[tag][term] = value; }
