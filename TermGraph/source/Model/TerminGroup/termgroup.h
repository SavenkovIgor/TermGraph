#pragma once

#include <QObject>

#include "source/Helpers/appstyle.h"
#include "source/Helpers/tagprocessor.h"
#include "source/Model/GraphicItem/rectgraphicitem.h"
#include "source/Model/TerminGroup/termgroupinfo.h"
#include "source/Model/TerminEdge/edge.h"
#include "source/Model/GraphicItem/labelgraphicitem.h"

class TermGroup : public QObject, public TermGroupInfo
{
    Q_OBJECT

    static int animSpeed;

    QTimer animTimer;
    QTimer checkSwapTimer;

public:
    TermGroup(const GroupInfoContainer& info, QObject* parent = nullptr);
    ~TermGroup();

    void initNewNodes();

    RectGraphicItem *baseRect;

    QString getNameLabel();
    QPointF getNamePos();
    QRectF getGroupRect() const;
    QColor getGroupColor();
    QColor getGroupFillColor();

    void setAnimSpeed(int val);
    static int getAnimSpeed();

    // Position and frame
    void setBasePoint(QPointF pt);
    void updateGroupFrame();

    void loadNodes(PaintedTerm::List newNodes);

    bool needPaint() const;
    void resetPaintFlags();

    PaintedTerm* getNodeAtPoint(const QPointF &pt) const;
    void setHover(QPointF mousePos);

public slots:
    void checkSwap();

    void startAnimation();
    void stopAnimation();

    void sceneUpdateSignal();

private slots:
    void animateGroup();

private:
    // Base init
    void addOrphansToParents();
    void addEdgesToParents();

    // Group
    qreal getGroupMinWidth();
    void updateRectsPositions();
    void updateBaseRectSize();

    // Tree
    void setTreeCoords();

    // Orphans
    void setOrphCoords(qreal maxWidth = 200.0);

    // Main objects
    LabelGraphicItem* grNmItem;
    void addTreeRectsToScene();
    RectGraphicItem *orphansRect;
};
