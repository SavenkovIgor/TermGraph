// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QFont>
#include <QFontMetricsF>
#include <QSizeF>

class Fonts
{
public:
    static QFont  defaultFont();
    static QFont  weightFont();
    static QSizeF metrics(const QString& text, const QFont& font = defaultFont());
};
