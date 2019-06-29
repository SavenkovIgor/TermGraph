#pragma once

#include <QSizeF>

#include "source/Model/enums.h"
#include "source/Model/GraphicItem/graphicitem.h"

class RectGraphicItem : public GraphicItem
{
public:
    // Constructors
    RectGraphicItem();
    explicit RectGraphicItem(const QPointF& pt, const QSizeF& getSize);

    // Destructor
    ~RectGraphicItem() override;

    void setSize(const QSizeF& getSize);

    QRectF getRect(CoordType coord) const;
    QSizeF getSize() const;

private:
    QSizeF _size;
};
