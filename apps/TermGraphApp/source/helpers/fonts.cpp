// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/fonts.h"

QFont Fonts::defaultFont()
{
    QFont font("Arial");
    return font;
}

QFont Fonts::weightFont()
{
    QFont font = defaultFont();
    font.setWeight(QFont::Black);
    return font;
}

// TODO: Check input with newline character
QSizeF Fonts::metrics(const QString& text, const QFont& font)
{
    QFontMetricsF mtr = QFontMetricsF(font);
    return mtr.boundingRect(text).size();
}
