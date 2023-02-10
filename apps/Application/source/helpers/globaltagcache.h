// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QMap>

#include <CommonTools/HandyTypes.h>


class GlobalTagCache
{
public:
    GlobalTagCache() = default;

    Opt<Opt<int>> get(const QString& tag, const QString& term) const;
    void          add(const QString& tag, const QString& term, Opt<int> value);

private:
    QMap<QString, QMap<QString, Opt<int>>> tagToTermCache;
};
