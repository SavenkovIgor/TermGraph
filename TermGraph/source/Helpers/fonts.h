#ifndef FONTS_H
#define FONTS_H

#include <QApplication>
#include <QFont>
#include <QFontMetricsF>
#include <QRectF>

class Fonts {
public:
    static QFont getFont();
    static QFont getWeightFont(int weight = QFont::Black);
    static QRectF getTextMetrics(const QString& text, const QFont& font = getFont());
};

#endif // FONTS_H
