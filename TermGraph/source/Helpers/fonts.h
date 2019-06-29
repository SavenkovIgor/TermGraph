#pragma once

#include <QApplication>
#include <QFont>
#include <QFontMetricsF>
#include <QSizeF>

class Fonts {
public:
    static QFont getFont();
    static QFont getWeightFont(int weight = QFont::Black);
    static QSizeF getTextMetrics(const QString& text, const QFont& font = getFont());
};
