#include "paintqueuemanager.h"

PaintQueueManager::PaintQueueManager() : QObject ()
{

}

void PaintQueueManager::clearGroupsQueue()
{
    groupsForPaint.clear();
}

void PaintQueueManager::addGroup(TermGroup *group)
{
    groupsForPaint.enqueue(group);
}

void PaintQueueManager::nextGroup()
{
    groupsForPaint.dequeue();
}

bool PaintQueueManager::groupQueueEmpty()
{
    return groupsForPaint.isEmpty();
}

QRectF PaintQueueManager::currentGroupRect()
{
    return groupsForPaint.head()->getGroupRect();
}

QColor PaintQueueManager::currentGroupFillColor()
{
    return groupsForPaint.head()->getGroupFillColor();
}

QString PaintQueueManager::currentGroupName()
{
    return groupsForPaint.head()->getName();
}

QPointF PaintQueueManager::currentGroupNamePos()
{
    return groupsForPaint.head()->getNamePos();
}

QStringList PaintQueueManager::currentGroupAllNodeNames()
{
    QStringList ret;
    for (auto node : groupsForPaint.head()->getAllNodes()) {
        ret << node->getSmallName();
    }
    return ret;
}

QList<QRectF> PaintQueueManager::currentGroupAllRects()
{
    QList<QRectF> ret;
    for (auto node : groupsForPaint.head()->getAllNodes()) {
        ret << node->getNodeRect(CoordType::scene);
    }
    return ret;
}

void PaintQueueManager::nextEdge()
{
    groupsForPaint.head()->edgesPaintQueue.dequeue();
}

bool PaintQueueManager::edgeQueueEmpty()
{
    return groupsForPaint.head()->edgesPaintQueue.isEmpty();
}

QColor PaintQueueManager::getEdgeColor()
{
    return AppStyle::Colors::Edges::termin;
}

QPointF PaintQueueManager::currentFirstEdgePoint()
{
    auto graphTerm = groupsForPaint.head()->edgesPaintQueue.head()->getRoot();
    PaintedTerm* paintedTerm = dynamic_cast<PaintedTerm*>(graphTerm);
    auto pt = paintedTerm->getScenePos();
    pt += paintedTerm->getNodeRect(CoordType::zeroPoint).center();
    return pt;
}

QPointF PaintQueueManager::currentLastEdgePoint()
{
    auto graphTerm = groupsForPaint.head()->edgesPaintQueue.head()->getLeaf();
    PaintedTerm* paintedTerm = dynamic_cast<PaintedTerm*>(graphTerm);
    auto pt = paintedTerm->getScenePos();
    pt += paintedTerm->getNodeRect(CoordType::zeroPoint).center();
    return pt;
}

void PaintQueueManager::nextNode()
{
    groupsForPaint.head()->nodesPaintQueue.dequeue();
}

bool PaintQueueManager::nodeQueueEmpty()
{
    return groupsForPaint.head()->nodesPaintQueue.isEmpty();
}

qreal PaintQueueManager::currentNodeRadius()
{
    return groupsForPaint.head()->nodesPaintQueue.head()->getCornerRadius();
}

QRectF PaintQueueManager::currentNodeRect()
{
    return groupsForPaint.head()->nodesPaintQueue.head()->getNodeRect(CoordType::scene);
}

QPointF PaintQueueManager::currentNodeCenter()
{
    return groupsForPaint.head()->nodesPaintQueue.head()->getNodeRect(CoordType::scene).center();
}

QColor PaintQueueManager::currentNodeColor()
{
    auto col = groupsForPaint.head()->nodesPaintQueue.head()->getColor();
    qDebug() << "col " << col;
    return col;
}

QString PaintQueueManager::currentNodeText()
{
    return groupsForPaint.head()->nodesPaintQueue.head()->getSmallName();
}
