#ifndef RECTGRAPHICITEM_H
#define RECTGRAPHICITEM_H

#include <QSizeF>

#include "./graphicitem.h"

class RectGraphicItem : public GraphicItem
{
public:
    // Constructors
    RectGraphicItem();
    explicit RectGraphicItem(const QPointF& pt, const QSizeF& size);

    // Destructor
    ~RectGraphicItem() override;

    virtual QRectF boundingRect() const override;

    void setSize(const QSizeF& size);

private:
    QSizeF _size;

    QRectF getRect() const;
};

#endif // RECTGRAPHICITEM_H
