#ifndef LABELGRAPHICITEM_H
#define LABELGRAPHICITEM_H

#include <QString>

#include "./graphicitem.h"

class LabelGraphicItem : public GraphicItem
{
public:
    LabelGraphicItem();
    ~LabelGraphicItem() override;

    void setLabel(const QString& label);

private:
    QString _label;
};

#endif // LABELGRAPHICITEM_H
