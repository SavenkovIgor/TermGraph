// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QMap>

class LinkTermDistanceCache
{
public:
    LinkTermDistanceCache() = default;

    std::optional<std::optional<int>> get(const QString& link, const QString& term) const;
    void                              add(const QString& link, const QString& term, std::optional<int> value);

private:
    QMap<QString, QMap<QString, std::optional<int>>> linkToTermCache;
};
