#include "labelgraphicitem.h"

LabelGraphicItem::LabelGraphicItem() : GraphicItem() { }

LabelGraphicItem::LabelGraphicItem(const QString &label, const QFont &font) :
    GraphicItem()
{
    setLabel(label);
    setFont(font);
}

LabelGraphicItem::~LabelGraphicItem() { }

void LabelGraphicItem::setLabel(const QString &label)
{
    _label = label;
}

void LabelGraphicItem::setFont(const QFont &font)
{
    _font = font;
}

QString LabelGraphicItem::getLabel() const
{
    return _label;
}

QSizeF LabelGraphicItem::getNameSize() const {
    return Fonts::getTextMetrics(_label, _font);
}

QRectF LabelGraphicItem::frameRect() const
{
    return QRectF(pos(), getNameSize());
}
