#include "fonts.h"

QFont Fonts::getFont()
{
    return qApp->font();
}

QFont Fonts::getWeightFont(int weight)
{
    QFont font = getFont();
    font.setWeight(weight);
    return font;
}

QRectF Fonts::getTextMetrics(QString text, QFont font)
{
    QFontMetricsF mtr = QFontMetricsF(font);
    return  mtr.boundingRect(text);
}
