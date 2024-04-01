// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QMap>

#include "source/CommonTools/HandyTypes.h"

class LinkTermDistanceCache
{
public:
    LinkTermDistanceCache() = default;

    Opt<Opt<int>> get(const QString& link, const QString& term) const;
    void          add(const QString& link, const QString& term, Opt<int> value);

private:
    QMap<QString, QMap<QString, Opt<int>>> linkToTermCache;
};
