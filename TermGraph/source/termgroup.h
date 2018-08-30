#ifndef TERMGROUP_H
#define TERMGROUP_H

#include <QObject>
#include <QtSql>

#include "./Helpers/appstyle.h"
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

    NodesList getAllNodes();
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

    void loadNodes(NodesList newNodes);
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
    NodesList getInTreeNodes();
    NodesList getRootNodes();
    void setLayers();
    void setTreeCoords();
    void setNeighbours();
    // Tree tools
    QSizeF getTheoreticalTreeSize();
    int getLayersCount() const;
    NodesList getNodesInLevel(int lev) const;
    NodesList sortNodesInLayer(NodesList lst);

    // Edges
    EdgesList searchConnections();  // TODO: Переделать эту функцию так же

    // Orphans
    NodesList getOrphanNodes();
    QSizeF getOrphansSize();
    void setOrphCoords(qreal maxWidth = 200.0);

    // Tools
    qreal getMaxHeightInAllLevels() const;
    QSizeF getVerticalStackedSize(NodesList lst) const;
    void swapNodes(TermNode *n1, TermNode *n2);

    void hideRect(QGraphicsRectItem* item);

    // Main objects
    TGroupName *grNmItem;
    QGraphicsRectItem *treeRect;
    QGraphicsRectItem *orphansRect;

    NodesList nodesList;
    EdgesList edgesList;

    QUuid groupUuid;
    GroupType type = freeEdges;
};

#endif  // TERMGROUP_H
