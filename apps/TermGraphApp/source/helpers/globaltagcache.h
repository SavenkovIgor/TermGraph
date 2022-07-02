// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QMap>

#include <CommonTools/HandyTypes.h>

class GlobalTagCache
{
public:
    static GlobalTagCache& instance();
    GlobalTagCache(GlobalTagCache const&) = delete;
    void operator=(GlobalTagCache const&) = delete;

    Opt<Opt<int>> get(const QString& tag, const QString& term);
    void          add(const QString& tag, const QString& term, Opt<int> value);

private:
    GlobalTagCache() = default;

    QMap<QString, QMap<QString, Opt<int>>> tagToTermCache;
};
