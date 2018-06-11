#ifndef TERMNODE_H
#define TERMNODE_H

#include <QMenu>
#include <QGraphicsItem>
#include <QDebug>

#include "glb.h"
#include "terminfo.h"

enum NodeType{
    orphan,
    root,
    endLeaf,
    middleLeaf
};

class TermNode :  public TermInfo, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    TermNode( QSqlRecord rec );          //Можно
    ~TermNode();

    enum KnowLevel{
        dontKnowLvl = 0,
        remindLvl,
        wellRemindLvl
    };    

    bool needRemindToday();
    bool isRemindDateMissed();
    void setRemind(KnowLevel lvl);

    void addEdgeRef(Edge *edge);

    static QList< Qt::Edge > sides;

    static DBAbstract *db;

    static const qreal verScale;
    static const qreal vInterv;
    static const qreal hInterv; //TODO: Вынести в sizes

    static bool someoneHover;
    static bool someoneSelect;
    bool relative = false;
    void setRelatPaint(bool val);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *evt);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *evt);


    QPointF getCenter();
    QRectF boundingRect() const;
    QRectF getMainRect( bool localPos = false ) const;
    QRectF getRcWithBorders();
    QSizeF getSize( bool withBorder = true );
    QLineF getRectLine( Qt::Edge sd );

    NodeType getNodeType();
    QColor getBaseColor();

    QString getGroupString();

    bool isLearning();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *);

    void mousePressEvent(QGraphicsSceneMouseEvent *evt);

    bool hasConnections();
    bool hasConnectionsInGroup();

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

    QRectF getTreeSize(QRectF rect);

    EdgesList getConnectedEdges();

    QString getDebugString();

    int getRepNum() const;

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

    void showInfo(QString name,QString group,QString descr,bool root);

    void startGroupAnimation();
    void stopGroupAnimation();

private slots:

    void swithcAtLearnVar();

private:
    // Initialize
    void adjustSizeForName();

    // Learning
    int getNextRepeatOffset(int lvl);
    int getLevelDaysFromBase(int lvl);

    // Positioning
    TermNode* getNearestLeftNeigh();
    TermNode* getNearestRightNeigh();
    EdgesList getEdgesInLayer();
    bool isNearPoints(QPointF pt1, QPointF pt2, qreal dist);

    // Colors
    const static QColor leafColor;
    const static QColor rootColor;
    const static QColor orphanColor;
    const static QColor selectedColor;

    // Hovers
    bool thisHovered = false;
    bool thisSelected = false;

    QRectF mainRect = QRectF(QPointF(0,0),QSize(40,10));

    bool atLearn;
    int repNum = -1;
    QDate lastRepeatDate;

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
