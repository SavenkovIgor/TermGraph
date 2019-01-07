#include "graphicitemterm.h"

GraphicItemTerm::GraphicItemTerm(QSqlRecord rec):
    PaintedTerm(rec),
    GraphicItem()
{
    adjustRectSizeForName();
    setZValue(1);
}

GraphicItemTerm::~GraphicItemTerm() { }

QRectF GraphicItemTerm::frameRect() const
{
    return getNodeRect(CoordType::zeroPoint);
}

QPointF GraphicItemTerm::getPos() const
{
    return this->pos();
}

QPointF GraphicItemTerm::getScenePos() const
{
    return this->scenePos();
}

void GraphicItemTerm::movePosBy(qreal dx, qreal dy)
{
    moveBy(dx,dy);
}

void GraphicItemTerm::setSceneParent(QGraphicsItem *item)
{
    this->setParentItem(item);
}
