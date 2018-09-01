#ifndef TERMNODE_H
#define TERMNODE_H

#include <QGraphicsItem>
#include <QDebug>

#include "./Helpers/handytypes.h"
#include "./edge.h"
#include "./Helpers/appstyle.h"
#include "./Helpers/helpstuff.h"
#include "./termgraph.h"

enum class NodeType {
    orphan,
    root,
    endLeaf,
    middleLeaf
};

enum class CoordType {
    none,
    local,
    scene
};

class TermNode:  public TermGraph, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    TermNode(QSqlRecord rec);
    ~TermNode();

    void addEdgeRef(Edge *edge);

    static QList< Qt::Edge > sides;

    static const qreal verScale;

    static bool someoneHover;
    static bool someoneSelect;
    bool relative = false;
    void setRelatPaint(bool val);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *evt);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *evt);

    QRectF getNodeRect(CoordType inCoordinates) const;
    QRectF getFrameRect(CoordType inCoordinates) const;

    QPointF getCenter(CoordType inCoordinates) const;

    QRectF boundingRect() const;
    QRectF getRcWithBorders();
    QSizeF getSize(bool withBorder = true);

    QLineF getRectLine(Qt::Edge side);

    NodeType getNodeType();
    QColor getBaseColor();
    QColor getSelectedColor();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *);

    void mousePressEvent(QGraphicsSceneMouseEvent *evt);


    void setLevel(int lev);
    int getUpLevels(int pLevel = -1);

    void clearNeighbours();
    void clearConnBrRootLists();

    void addToNeighboursList(TermNode *t);

    int getPaintLevel();

    void countForces();
    int  getIntersections(bool swapped  = false);
    qreal getSumEdgesLength(bool swap);
    void setSwap(QPointF toPt);
    void dropSwap();

    bool applyMove();


    QString getDebugString();

    bool isRoot();
    bool isOrphan();
    bool isLeaf();

    bool isInTree();

private:
    // Info
    bool hasConnections();
    bool hasConnectionsInGroup();

    // Initialize
    void adjustSizeForName();

    // Positioning
    TermNode* getNearestLeftNeigh();
    TermNode* getNearestRightNeigh();
    EdgesList getEdgesInLayer();
    bool isNearPoints(QPointF pt1, QPointF pt2, qreal dist);

    // Hovers
    bool thisHovered = false;
    bool thisSelected = false;

    QSizeF nodeSize = QSizeF(40.0, 10.0);
    QRectF getInnerNodeRect() const;

    QString testStr;

    // Paint / Animation
    int paintLevel = -1;
    qreal newPosOffs = 0.0;
};

#endif  // TERMNODE_H
