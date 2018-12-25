#include "paintqueuemanager.h"

PaintManager::PaintManager() : QObject ()
{

}

void PaintManager::sendPaintNodeSignal()
{
    paintNodeQueue();
}

void PaintManager::sendPaintGroupSignal()
{
    paintGroupQueue();
}

void PaintManager::addClearRect(const QRectF &rect, bool paintNow)
{
    clearPaintArea.enqueue(rect);

    if (paintNow) {
        paintGroupQueue();
    }
}

QRectF PaintManager::currentClearRect()
{
    return clearPaintArea.head();
}

void PaintManager::nextClearRect()
{
    clearPaintArea.dequeue();
}

bool PaintManager::clearQueueEmpty() const
{
    return clearPaintArea.isEmpty();
}

void PaintManager::clearAllQueues()
{
    groupRectsForPaint.clear();
    groupNamesForPaint.clear();
    nodesForPaint.clear();
    edgesForPaint.clear();
}

void PaintManager::addGroup(TermGroup *group, bool ignoreNeedPaintFlag, bool paintNow)
{
    groupRectsForPaint.enqueue(QPair<QRectF, QColor>(group->getGroupRect(), group->getGroupFillColor()));
    groupNamesForPaint.enqueue(QPair<QPointF, QString>(group->getNamePos(), group->getName()));

    for (auto edge : group->getAllEdgesForPainting()) {
        if (ignoreNeedPaintFlag) {
            edgesForPaint.enqueue(edge);
        } else {
            if (edge->needPaint) {
                edgesForPaint.enqueue(edge);
                edge->needPaint = false;
            }
        }
    }

    for (auto node : group->getAllNodes()) {
        if (ignoreNeedPaintFlag) {
            nodesForPaint.enqueue(node);
        } else {
            if (node->needPaint) {
                nodesForPaint.enqueue(node);
                node->needPaint = false;
            }
        }
    }

    if (paintNow) {
        paintGroupQueue();
    }
}

void PaintManager::addNode(GraphicItemTerm *node, bool paintNow)
{
    nodesForPaint.enqueue(node);
    if (paintNow) {
        paintNodeQueue();
    }
}

void PaintManager::nextGroupRect()
{
    groupRectsForPaint.dequeue();
}

bool PaintManager::groupRectQueueEmpty() const
{
    return groupRectsForPaint.isEmpty();
}

QRectF PaintManager::currentGroupRect()
{
    return groupRectsForPaint.head().first;
}

QColor PaintManager::currentGroupFillColor()
{
    return groupRectsForPaint.head().second;
}

void PaintManager::nextGroupName()
{
    groupNamesForPaint.dequeue();
}

bool PaintManager::groupNamesQueueEmpty() const
{
    return groupNamesForPaint.isEmpty();
}

QString PaintManager::currentGroupName()
{
    return groupNamesForPaint.head().second;
}

QPointF PaintManager::currentGroupNamePos()
{
    return groupNamesForPaint.head().first;
}

void PaintManager::nextEdge()
{
    edgesForPaint.dequeue();
}

bool PaintManager::edgeQueueEmpty() const
{
    return edgesForPaint.isEmpty();
}

QColor PaintManager::getEdgeColor()
{
    return edgesForPaint.head()->getEdgeColor();
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

bool PaintManager::nodeQueueEmpty() const
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

void PaintManager::startCheckTimer()
{
    paintSpeedTimer.start();
}

void PaintManager::restartCheckTimer(QString label)
{
    qDebug() << label << " " << paintSpeedTimer.restart();
}
