// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QFont>
#include <QFontMetricsF>
#include <QSizeF>

export module Text.Fonts;

export class Fonts
{
public:
    static QFont defaultFont()
    {
        QFont font("Arial");
        font.setPointSize(11);
        return font;
    }

    static QFont weightFont()
    {
        QFont font = defaultFont();
        font.setWeight(QFont::Black);
        return font;
    }

    static QSizeF metrics(const QString& text, const QFont& font = defaultFont())
    {
        QFontMetricsF mtr = QFontMetricsF(font);
        return mtr.boundingRect(text).size();
    }
};
