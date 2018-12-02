#ifndef PAINTQUEUEMANAGER_H
#define PAINTQUEUEMANAGER_H

#include <QObject>
#include <QQueue>

#include "./Model/TerminGroup/termgroup.h"

class PaintQueueManager : public QObject
{
    Q_OBJECT

public:
    PaintQueueManager();

public slots:
    // Groups
    void clearGroupsQueue();
    void addGroup(TermGroup* group);
    void nextGroup();
    bool groupQueueEmpty();

    QRectF currentGroupRect();
    QColor currentGroupFillColor();
    QString currentGroupName();
    QPointF currentGroupNamePos();
    QStringList currentGroupAllNodeNames();
    QList<QRectF> currentGroupAllRects();

    // Edges
    void nextEdge();
    bool edgeQueueEmpty();

    QColor getEdgeColor();
    QPointF currentFirstEdgePoint();
    QPointF currentLastEdgePoint();

    // Nodes
    void nextNode();
    bool nodeQueueEmpty();

    qreal currentNodeRadius();
    QRectF currentNodeRect();
    QPointF currentNodeCenter();
    QColor currentNodeColor();
    QString currentNodeText();

private:
    QQueue < TermGroup* > groupsForPaint;
};

#endif // PAINTQUEUEMANAGER_H
