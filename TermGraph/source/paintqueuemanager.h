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
    void sendPaintNodeSignal();

signals:
    void paintGroupQueue();
    void paintNodeQueue();

    void clearAll();

public slots:
    // Fill functions
    void clearAllQueues();
    void addGroup(TermGroup* group, bool ignoreNeedPaintFlag = false, bool paintNow = true);
    void addNode(GraphicItemTerm* node, bool paintNow = true);

    // GroupRects
    void nextGroupRect();
    bool groupRectQueueEmpty();
    QRectF currentGroupRect();
    QColor currentGroupFillColor();

    // GroupNames
    void nextGroupName();
    bool groupNamesQueueEmpty();
    QString currentGroupName();
    QPointF currentGroupNamePos();

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
    QQueue < QPair<QRectF, QColor> > groupRectsForPaint;
    QQueue < QPair<QPointF, QString> > groupNamesForPaint;
    QQueue <GraphicItemTerm*> nodesForPaint;
    QQueue <Edge*> edgesForPaint;
};

#endif // PAINTQUEUEMANAGER_H
