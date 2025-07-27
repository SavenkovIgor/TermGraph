// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QMap>

export module LinkTermDistanceCache;

export class LinkTermDistanceCache
{
public:
    LinkTermDistanceCache() = default;

    std::optional<std::optional<int>> get(const QString& link, const QString& term) const
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

    void add(const QString& link, const QString& term, std::optional<int> value)
    {
        linkToTermCache[link][term] = value;
    }

private:
    QMap<QString, QMap<QString, std::optional<int>>> linkToTermCache;
};
