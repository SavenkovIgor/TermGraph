#ifndef TERMNODE_H
#define TERMNODE_H

#include <QMenu>
#include <QGraphicsItem>
#include <QDebug>

#include "glb.h"
#include "terminfo.h"

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

class TermNode:  public TermInfo, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    TermNode(QSqlRecord rec);
    TermNode(QJsonObject jsonObject);
    ~TermNode();

    void addEdgeRef(Edge *edge);

    static QList< Qt::Edge > sides;

    static DBAbstract *db;

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
    QSizeF getSize( bool withBorder = true );

    QLineF getRectLine( Qt::Edge sd );

    NodeType getNodeType();
    QColor getBaseColor();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *);

    void mousePressEvent(QGraphicsSceneMouseEvent *evt);


    void setLevel(int lev);
    int getUpLevels( int pLevel = -1 );

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
//    QString getSmallName() {
//        if( isRoot() )
//            return TermInfo::getName() + "true";
//        else
//            return TermInfo::getName() + "false";
//    }

signals:
    void startGroupAnimation();
    void stopGroupAnimation();

private:
    // Info
    bool hasConnections();
    bool hasConnectionsInGroup();
    EdgesList getConnectedEdges();

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

    QSizeF nodeSize = QSizeF(40.0,10.0);
    QRectF getInnerNodeRect() const;

    QString testStr;

    // Paint / Animation
    int paintLevel = -1;
    qreal newPosOffs = 0.0;

    NodesList rootNodes;
    NodesList leafNodes;
    NodesList neighbourNodes;

    EdgesList edgesToOtherGroups; //Пока никак не используются но будут.
    EdgesList edgesToLeafs;
    EdgesList edgesToRoots;
};

#endif // TERMNODE_H
