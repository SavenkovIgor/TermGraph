#ifndef PAINTQUEUEMANAGER_H
#define PAINTQUEUEMANAGER_H

#include <QObject>
#include <QQueue>

#include "./Model/TerminGroup/termgroup.h"

class PaintManager : public QObject
{
    Q_OBJECT

public:
    PaintManager() = default;
    ~PaintManager() = default;

    void sendPaintNodeSignal();
    void sendPaintGroupSignal();

signals:
    void paintGroupQueue();
    void paintNodeQueue();

public slots:
    // Clear functions
    void addClearRect(const QRectF& rect, bool paintNow = false);
    QRectF currentClearRect();
    void nextClearRect();
    bool clearQueueEmpty() const;

    // Fill functions
    void clearAllQueues();
    void addGroup(TermGroup* group, bool ignoreNeedPaintFlag = false, bool paintNow = true);
    void addRect(const QRectF& rect, const QColor& color = Qt::white);
    void addNode(PaintedTerm* node, bool paintNow = true);

    // GroupRects
    void nextGroupRect();
    bool groupRectQueueEmpty() const;

    QRectF currentGroupRect() const;
    QColor currentGroupFillColor() const;

    // GroupNames
    void nextGroupName();
    bool groupNamesQueueEmpty() const;

    QString currentGroupName() const;
    QPointF currentGroupNamePos() const;

    // Edges
    void nextEdge();
    bool edgeQueueEmpty() const;

    QColor getEdgeColor() const;
    QPointF currentFirstEdgePoint() const;
    QPointF currentLastEdgePoint() const;

    // Nodes
    void nextNode();
    bool nodeQueueEmpty() const;

    qreal currentNodeRadius() const;
    QRectF currentNodeRect() const;
    QPointF currentNodeCenter() const;
    QColor currentNodeColor() const;
    QString currentNodeText() const;

    // Flags
    void setPaintInProcessFlag(bool paintNow);
    bool isPaintInProcessNow() const;

    // Paint speed check
    void startCheckTimer();
    void restartCheckTimer(const QString& label = "paint speed");

private:
    bool paintInProcessFlag = false;

    // Paint queues
    QQueue <QRectF> clearPaintArea;
    QQueue < QPair<QRectF, QColor> > groupRectsForPaint;
    QQueue < QPair<QPointF, QString> > groupNamesForPaint;
    QQueue <PaintedTerm*> nodesForPaint;
    QQueue <Edge*> edgesForPaint;

    // Timer
    QElapsedTimer paintSpeedTimer;
};

#endif // PAINTQUEUEMANAGER_H
