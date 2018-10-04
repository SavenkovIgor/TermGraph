#ifndef TERMGROUP_H
#define TERMGROUP_H

#include <QObject>
#include <QtSql>

#include "../databaseWorks/termgrouptbl.h"
#include "../Helpers/appstyle.h"
#include "../Helpers/tagprocessor.h"
#include "./Termin/graphicitemterm.h"
#include "./termgroupinfo.h"
#include "./tgroupname.h"
#include "./TerminEdge/edge.h"

class TermGroup : public QObject, public TermGroupInfo
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

    QString getTypeString();
    static QStringList getTypesNames();
    static QMap<GroupType, QString> getTypesMap();

    QString getName();

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

    void loadNodes(GraphicItemTerm::List newNodes);
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
    GraphicItemTerm::List getInTreeNodes();
    GraphicItemTerm::List getRootNodes();
    void setLayers();
    void setTreeCoords();
    void setNeighbours();
    // Tree tools
    QSizeF getTheoreticalTreeSize();
    int getLayersCount() const;
    GraphicItemTerm::List getNodesInLevel(int lev) const;
    GraphicItemTerm::List sortNodesInLayer(GraphicItemTerm::List lst);

    // Edges
    Edge* addNewEdge(GraphicItemTerm* node1, GraphicItemTerm* node2);
    EdgesList searchConnections();  // TODO: Переделать эту функцию так же

    // Orphans
    GraphicItemTerm::List getOrphanNodes();
    QSizeF getOrphansSize();
    void setOrphCoords(qreal maxWidth = 200.0);

    // Tools
    qreal getMaxHeightInAllLevels() const;
    QSizeF getVerticalStackedSize(GraphicItemTerm::List lst) const;
    void swapNodes(GraphicItemTerm *n1, GraphicItemTerm *n2);

    void hideRect(QGraphicsRectItem* item);

    // Main objects
    TGroupName *grNmItem;
    QGraphicsRectItem *treeRect;
    QGraphicsRectItem *orphansRect;

    EdgesList edgesList;
};

#endif  // TERMGROUP_H
