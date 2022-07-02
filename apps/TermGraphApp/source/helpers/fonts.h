// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QFont>
#include <QFontMetricsF>
#include <QSizeF>

class Fonts
{
public:
    static QFont  getFont();
    static QFont  getWeightFont(const QFont::Weight weight = QFont::Black);
    static QSizeF getTextMetrics(const QString& text, const QFont& font = getFont());
};
