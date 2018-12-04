#include "paintqueuemanager.h"

PaintManager::PaintManager() : QObject ()
{

}

void PaintManager::clearGroupsQueue()
{
    groupsForPaint.clear();
}

void PaintManager::addGroup(TermGroup *group, bool sendPaintSignal)
{
    groupsForPaint.enqueue(group);
    if (sendPaintSignal) {
        paintGroupQueue();
    }
}

void PaintManager::addNode(GraphicItemTerm *node, bool sendPaintSignal)
{
    nodesForPaint.enqueue(node);
    if (sendPaintSignal) {
        paintNodeQueue();
    }
}

void PaintManager::fillNodeAndEdgeQueuesFromCurrentGroup()
{
    for (auto edge : groupsForPaint.head()->getAllEdges()) {
        if (edge->needPaint) {
            edgesForPaint.enqueue(edge);
            edge->needPaint = false;
        }
    }

    for (auto node : groupsForPaint.head()->getAllNodes()) {
        if (node->needPaint) {
            nodesForPaint.enqueue(node);
            node->needPaint = false;
        }
    }
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
    edgesForPaint.dequeue();
}

bool PaintManager::edgeQueueEmpty()
{
    return edgesForPaint.isEmpty();
}

QColor PaintManager::getEdgeColor()
{
    return AppStyle::Colors::Edges::termin;
}

QPointF PaintManager::currentFirstEdgePoint()
{
    auto graphTerm = edgesForPaint.head()->getRoot();
    PaintedTerm* paintedTerm = dynamic_cast<PaintedTerm*>(graphTerm);
    auto pt = paintedTerm->getScenePos();
    pt += paintedTerm->getNodeRect(CoordType::zeroPoint).center();
    return pt;
}

QPointF PaintManager::currentLastEdgePoint()
{
    auto graphTerm = edgesForPaint.head()->getLeaf();
    PaintedTerm* paintedTerm = dynamic_cast<PaintedTerm*>(graphTerm);
    auto pt = paintedTerm->getScenePos();
    pt += paintedTerm->getNodeRect(CoordType::zeroPoint).center();
    return pt;
}

void PaintManager::nextNode()
{
    nodesForPaint.dequeue();
}

bool PaintManager::nodeQueueEmpty()
{
    return nodesForPaint.isEmpty();
}

qreal PaintManager::currentNodeRadius()
{
    return nodesForPaint.head()->getCornerRadius();
}

QRectF PaintManager::currentNodeRect()
{
    return nodesForPaint.head()->getNodeRect(CoordType::scene);
}

QPointF PaintManager::currentNodeCenter()
{
    return nodesForPaint.head()->getNodeRect(CoordType::scene).center();
}

QColor PaintManager::currentNodeColor()
{
    return nodesForPaint.head()->getColor();
}

QString PaintManager::currentNodeText()
{
    return nodesForPaint.head()->getSmallName();
}

void PaintManager::setPaintInProcessFlag(bool paintNow)
{
    paintInProcessFlag = paintNow;
}

bool PaintManager::isPaintInProcessNow()
{
    return paintInProcessFlag;
}
