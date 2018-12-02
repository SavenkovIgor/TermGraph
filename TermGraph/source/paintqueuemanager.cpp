#include "paintqueuemanager.h"

PaintManager::PaintManager() : QObject ()
{

}

void PaintManager::clearGroupsQueue()
{
    groupsForPaint.clear();
}

void PaintManager::addGroup(TermGroup *group)
{
    groupsForPaint.enqueue(group);
}

void PaintManager::nextGroup()
{
    groupsForPaint.dequeue();
}

bool PaintManager::groupQueueEmpty()
{
    return groupsForPaint.isEmpty();
}

QRectF PaintManager::currentGroupRect()
{
    return groupsForPaint.head()->getGroupRect();
}

QColor PaintManager::currentGroupFillColor()
{
    return groupsForPaint.head()->getGroupFillColor();
}

QString PaintManager::currentGroupName()
{
    return groupsForPaint.head()->getName();
}

QPointF PaintManager::currentGroupNamePos()
{
    return groupsForPaint.head()->getNamePos();
}

QStringList PaintManager::currentGroupAllNodeNames()
{
    QStringList ret;
    for (auto node : groupsForPaint.head()->getAllNodes()) {
        ret << node->getSmallName();
    }
    return ret;
}

QList<QRectF> PaintManager::currentGroupAllRects()
{
    QList<QRectF> ret;
    for (auto node : groupsForPaint.head()->getAllNodes()) {
        ret << node->getNodeRect(CoordType::scene);
    }
    return ret;
}

void PaintManager::nextEdge()
{
    groupsForPaint.head()->edgesPaintQueue.dequeue();
}

bool PaintManager::edgeQueueEmpty()
{
    return groupsForPaint.head()->edgesPaintQueue.isEmpty();
}

QColor PaintManager::getEdgeColor()
{
    return AppStyle::Colors::Edges::termin;
}

QPointF PaintManager::currentFirstEdgePoint()
{
    auto graphTerm = groupsForPaint.head()->edgesPaintQueue.head()->getRoot();
    PaintedTerm* paintedTerm = dynamic_cast<PaintedTerm*>(graphTerm);
    auto pt = paintedTerm->getScenePos();
    pt += paintedTerm->getNodeRect(CoordType::zeroPoint).center();
    return pt;
}

QPointF PaintManager::currentLastEdgePoint()
{
    auto graphTerm = groupsForPaint.head()->edgesPaintQueue.head()->getLeaf();
    PaintedTerm* paintedTerm = dynamic_cast<PaintedTerm*>(graphTerm);
    auto pt = paintedTerm->getScenePos();
    pt += paintedTerm->getNodeRect(CoordType::zeroPoint).center();
    return pt;
}

void PaintManager::nextNode()
{
    groupsForPaint.head()->nodesPaintQueue.dequeue();
}

bool PaintManager::nodeQueueEmpty()
{
    return groupsForPaint.head()->nodesPaintQueue.isEmpty();
}

qreal PaintManager::currentNodeRadius()
{
    return groupsForPaint.head()->nodesPaintQueue.head()->getCornerRadius();
}

QRectF PaintManager::currentNodeRect()
{
    return groupsForPaint.head()->nodesPaintQueue.head()->getNodeRect(CoordType::scene);
}

QPointF PaintManager::currentNodeCenter()
{
    return groupsForPaint.head()->nodesPaintQueue.head()->getNodeRect(CoordType::scene).center();
}

QColor PaintManager::currentNodeColor()
{
    auto col = groupsForPaint.head()->nodesPaintQueue.head()->getColor();
    qDebug() << "col " << col;
    return col;
}

QString PaintManager::currentNodeText()
{
    return groupsForPaint.head()->nodesPaintQueue.head()->getSmallName();
}
