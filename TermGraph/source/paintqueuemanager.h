#ifndef PAINTQUEUEMANAGER_H
#define PAINTQUEUEMANAGER_H

#include <QObject>
#include <QQueue>

#include "./Model/TerminGroup/termgroup.h"

class PaintManager : public QObject
{
    Q_OBJECT

public:
    PaintManager();

    void sendCleanAllSignal();
signals:
    void paintGroupQueue();
    void paintNodeQueue();

    void cleanAll();

public slots:
    // Groups
    void clearGroupsQueue();
    void addGroup(TermGroup* group, bool sendPaintSignal = true);
    void addNode(GraphicItemTerm* node, bool sendPaintSignal = true);
    void fillNodeAndEdgeQueuesFromCurrentGroup();
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

    // Flags
    void setPaintInProcessFlag(bool paintNow);
    bool isPaintInProcessNow();

private:
    bool paintInProcessFlag = false;

    // Paint queues
    QQueue < TermGroup* > groupsForPaint;
    QQueue <GraphicItemTerm*> nodesForPaint;
    QQueue <Edge*> edgesForPaint;
};

#endif // PAINTQUEUEMANAGER_H
