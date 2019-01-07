#include "labelgraphicitem.h"

LabelGraphicItem::LabelGraphicItem() : GraphicItem () { }

LabelGraphicItem::LabelGraphicItem(const QString &label) :
    GraphicItem ()
{
    setLabel(label);
}

LabelGraphicItem::~LabelGraphicItem() { }

void LabelGraphicItem::setLabel(const QString &label)
{
    _label = label;
}
