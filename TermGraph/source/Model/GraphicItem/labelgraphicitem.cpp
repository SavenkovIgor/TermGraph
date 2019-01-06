#include "labelgraphicitem.h"

LabelGraphicItem::LabelGraphicItem() : GraphicItem () { }
LabelGraphicItem::~LabelGraphicItem() { }

void LabelGraphicItem::setLabel(const QString &label)
{
    _label = label;
}
