#ifndef GRAPHICITEM_H
#define GRAPHICITEM_H

#include <QPointF>

class GraphicItem
{
public:
    GraphicItem();

    // Parents
    void setParentItem(GraphicItem* item);

    // Position
    void setPos(QPointF pt);
    QPointF pos() const;
    QPointF scenePos() const;

private:
    QPointF position;

    GraphicItem* parentItem = nullptr;
};

#endif // GRAPHICITEM_H
