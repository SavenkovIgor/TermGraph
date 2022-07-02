// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/fonts.h"

QFont Fonts::getFont()
{
    QFont font("Arial");
    return font;
}

QFont Fonts::getWeightFont(const QFont::Weight weight)
{
    QFont font = getFont();
    font.setWeight(weight);
    return font;
}

// TODO: Check input with newline character
QSizeF Fonts::getTextMetrics(const QString& text, const QFont& font)
{
    QFontMetricsF mtr = QFontMetricsF(font);
    return mtr.boundingRect(text).size();
}
