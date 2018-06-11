#ifndef TERMGROUP_H
#define TERMGROUP_H

#include <QObject>
#include <QtSql>

#include "databaseWorks/dbabstract.h"
#include "termnode.h"
#include "tgroupname.h"
#include "edge.h"

enum GroupType {
    freeEdges = -1,
    terms = 0
};

class TermGroup : public QObject
{
    Q_OBJECT

    static int animSpeed;

    void loadTermNodes();

    NodesList getRootList();
    NodesList getNodesInLevel(int lev) const;

    NodesList getOrphansList();
    bool      hasOrphans();

    NodesList getInTreeList();

    NodesList   nodeList;

    QTimer animTimer;
    QTimer checkSwapTimer;

    bool lockForce = true;

    QPropertyAnimation swAnim1;
    QPropertyAnimation swAnim2;

    QParallelAnimationGroup animGrp;

    int currAnimLevel = -1;

    qreal getGroupMinWidth();
    qreal getTitleMinWidth();
    qreal getOrphansMinWidth();
    qreal getTreeMinWidth();

public:
    static DBAbstract *db;

    explicit TermGroup(QSqlRecord rec, QObject *parent = 0);
    ~TermGroup();

    GroupType getType();
    QString getTypeString();
    static QStringList getTypesNames();
    static QMap<GroupType,QString> getTypesMap();

    int     getGroupUid();

    QString getName();

    NodesList getNodeList();

    void setAnimSpeed(int val);
    static int getAnimSpeed();

    QGraphicsRectItem *baseRect;
    void setBasePoint(QPointF pt);
private:
    void updateRectsPositions();
    void updateBaseRectSize();
public:

    TGroupName *grNmItem;
    QGraphicsRectItem *treeRect;
    QGraphicsRectItem *orphansRect;

//    QGraphicsRectItem *centerRect;

    QRectF getAllGroupRect( bool withOrph = true );
    QSizeF getOrphansSize();

    bool   hasTree();
    QSizeF getTheoreticalTreeSize();

    void setOrphCoords(qreal maxWidth = 200.0);
    void setLevels();
    void setTreeCoords();
    NodesList sortNodesInLayer(NodesList lst);
    void setNeighbours();
    void updGroupFrame();
    void centralizeTree();
    //TODO: Переделать эту функцию так же
    EdgesList searchConnections();
    void addNodesToParents();
    //TODO: доделать эту функцию и добавить еще сверху
    void suggestConnections();

    QSizeF getVerticalStackedSize(NodesList lst) const;
    int getLayersCount() const;
    qreal getMaxHeightInAllLevels() const;

    void swapNodes( TermNode *n1, TermNode *n2 );

    QJsonDocument getJsonDoc();

public slots:
    void checkSwap();

    void startAnimation();
    void stopAnimation();

private slots:
    void animateGroup();

private:
    void hideRect(QGraphicsRectItem* item);

    int grUid = -1;
    GroupType type = freeEdges;
    QString longUid = "";

};

#endif // TERMGROUP_H
