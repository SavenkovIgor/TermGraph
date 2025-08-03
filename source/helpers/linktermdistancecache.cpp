// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QMap>

export module LinkTermDistanceCache;

using namespace std;

export class LinkTermDistanceCache
{
public:
    LinkTermDistanceCache() = default;

    optional<optional<int>> get(const QString& link, const QString& term) const
    {
        auto linkIter = linkToTermCache.constFind(link);
        if (linkIter != linkToTermCache.constEnd()) {
            auto termIter = linkIter.value().constFind(term);
            if (termIter != linkIter.value().constEnd()) {
                return optional<optional<int>>(termIter.value());
            }
        }

        return nullopt;
    }

    void add(const QString& link, const QString& term, optional<int> value) { linkToTermCache[link][term] = value; }

private:
    QMap<QString, QMap<QString, optional<int>>> linkToTermCache;
};
