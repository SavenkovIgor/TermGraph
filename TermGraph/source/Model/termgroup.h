#ifndef TERMGROUP_H
#define TERMGROUP_H

#include <QObject>
#include <QtSql>

#include "../databaseWorks/termgrouptbl.h"
#include "../Helpers/appstyle.h"
#include "../Helpers/tagprocessor.h"
#include "./termnode.h"
#include "./tgroupname.h"
#include "./edge.h"

enum GroupType {  // TODO: class!
    freeEdges = -1,
    terms = 0
};

class TermGroup : public QObject
{
    Q_OBJECT

    static int animSpeed;

    QTimer animTimer;
    QTimer checkSwapTimer;

    bool lockForce = true;

    QPropertyAnimation swAnim1;
    QPropertyAnimation swAnim2;
    QParallelAnimationGroup animGrp;

    int currAnimLevel = -1;

public:
    explicit TermGroup(QSqlRecord rec, QObject* parent = nullptr);
    TermGroup(QJsonDocument doc, QObject* parent = nullptr);
    ~TermGroup();

    static bool checkJson(QJsonDocument doc);

    void initNewNodes();

    QGraphicsRectItem *baseRect;

    GroupType getType();
    QString getTypeString();
    static QStringList getTypesNames();
    static QMap<GroupType, QString> getTypesMap();

    QString getName();
    QUuid getUuid();

    TermNode::List getAllNodes();
    EdgesList getAllEdges();

    void setAnimSpeed(int val);
    static int getAnimSpeed();

//    QGraphicsRectItem *centerRect;

    // Position and frame
    void setBasePoint(QPointF pt);
    void updateGroupFrame();

    // Connections search
    void suggestConnections();  // TODO: доделать эту функцию

    QJsonDocument getJsonDoc();

    void loadNodes(TermNode::List newNodes);
public slots:
    void checkSwap();

    void startAnimation();
    void stopAnimation();

    void sceneUpdateSignal();

private slots:
    void animateGroup();

private:
    // Base init
    void addNodesToParents();
    void addEdgesToParents();

    // Group
    qreal getGroupMinWidth();
    void updateRectsPositions();
    void updateBaseRectSize();
    void loadEdges();

    // Tree
    TermNode::List getInTreeNodes();
    TermNode::List getRootNodes();
    void setLayers();
    void setTreeCoords();
    void setNeighbours();
    // Tree tools
    QSizeF getTheoreticalTreeSize();
    int getLayersCount() const;
    TermNode::List getNodesInLevel(int lev) const;
    TermNode::List sortNodesInLayer(TermNode::List lst);

    // Edges
    Edge* addNewEdge(TermNode* node1, TermNode* node2);
    EdgesList searchConnections();  // TODO: Переделать эту функцию так же

    // Orphans
    TermNode::List getOrphanNodes();
    QSizeF getOrphansSize();
    void setOrphCoords(qreal maxWidth = 200.0);

    // Tools
    qreal getMaxHeightInAllLevels() const;
    QSizeF getVerticalStackedSize(TermNode::List lst) const;
    void swapNodes(TermNode *n1, TermNode *n2);

    void hideRect(QGraphicsRectItem* item);

    // Main objects
    TGroupName *grNmItem;
    QGraphicsRectItem *treeRect;
    QGraphicsRectItem *orphansRect;

    TermNode::List nodesList;
    EdgesList edgesList;

    QUuid groupUuid;
    GroupType type = freeEdges;
};

#endif  // TERMGROUP_H
