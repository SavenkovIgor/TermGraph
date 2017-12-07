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
    NodesList getLevList(int lev);
    NodesList getOrphList();
    NodesList getInTreeList();

    bool      hasOrphans();

    NodesList   nodeList;

    qreal getSumSize (NodesList lst , bool withMargins = false, Qt::Orientation ori = Qt::Vertical );
    qreal getMaxSideSize( NodesList lst, Qt::Orientation ori = Qt::Vertical );

    QTimer animTimer;
    QTimer checkSwapTimer;

    bool lockForce = true;

    QPropertyAnimation swAnim1;
    QPropertyAnimation swAnim2;

    QParallelAnimationGroup animGrp;

    int currAnimLevel = -1;

public:


    static DBAbstract *db;

    explicit TermGroup(QString grpName, int type, QObject *parent = 0);
    ~TermGroup();

    GroupType getType();
    QString getTypeString();
    static QStringList       getTypesNames();
    static QMap<GroupType,QString> getTypesMap();


    int     getGroupUid();
    QString getName();

    NodesList getNodeList();

    void setAnimSpeed(int val);
    static int getAnimSpeed();

    QGraphicsRectItem *baseRect;
    void setBasePoint(QPointF pt);

    TGroupName *grNmItem;
    QGraphicsRectItem *groupRect;
    QGraphicsRectItem *treeRect;
    QGraphicsRectItem *centerRect;

    QRectF getAllGroupRect( bool withOrph = true );
    QRectF getOrphansRect();

    bool   hasTree();
    QRectF getTreeGeometry();

    void setOrphCoords();
    void setLevels();
    void setTreeCoords();
    void setNeighbours();
    void updGroupFrame();
    void centralizeTree();
    //TODO: Переделать эту функцию так же
    EdgesList searchConnections();
    //TODO: доделать эту функцию и добавить еще сверху
    void suggestConnections();

    int getAllLevelsCount() const;

    void swapNodes( TermNode *n1, TermNode *n2 );


public slots:
    void checkSwap();

    void startAnimation();
    void stopAnimation();

private slots:
    void animateGroup();

private:

    int grUid = -1;
    GroupType type = freeEdges;

};

#endif // TERMGROUP_H
