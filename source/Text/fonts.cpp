// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/Text/Fonts.h"

QFont Fonts::defaultFont()
{
    QFont font("Arial");
    font.setPointSize(11);
    return font;
}

QFont Fonts::weightFont()
{
    QFont font = defaultFont();
    font.setWeight(QFont::Black);
    return font;
}

QSizeF Fonts::metrics(const QString& text, const QFont& font)
{
    QFontMetricsF mtr = QFontMetricsF(font);
    return mtr.boundingRect(text).size();
}
