#include "fonts.h"

QFont Fonts::getFont()
{
    QFont font("Arial");
    return font;
}

QFont Fonts::getWeightFont(int weight)
{
    QFont font = getFont();
    font.setWeight(weight);
    return font;
}

// TODO: Check input with newline character
QSizeF Fonts::getTextMetrics(const QString& text, const QFont& font)
{
    QFontMetricsF mtr = QFontMetricsF(font);
    return  mtr.boundingRect(text).size();
}
