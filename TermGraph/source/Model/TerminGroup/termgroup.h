#ifndef TERMGROUP_H
#define TERMGROUP_H

#include <QObject>
#include <QtSql>
#include <QGraphicsRectItem>

#include "../../databaseWorks/termgrouptbl.h"
#include "../../Helpers/appstyle.h"
#include "../../Helpers/tagprocessor.h"
#include "./termgroupinfo.h"
#include "./tgroupname.h"
#include "../TerminEdge/edge.h"

class TermGroup : public QObject, public TermGroupInfo
{
    Q_OBJECT

    static int animSpeed;

    QTimer animTimer;
    QTimer checkSwapTimer;

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
    QPointF getNamePos();
    QRectF getGroupRect() const;
    QColor getGroupColor();
    QColor getGroupFillColor();

    void setAnimSpeed(int val);
    static int getAnimSpeed();

//    QGraphicsRectItem *centerRect;

    // Position and frame
    void setBasePoint(QPointF pt);
    void updateGroupFrame();

    QJsonDocument getJsonDoc();

    void loadNodes(GraphicItemTerm::List newNodes);

    bool alreadyPainted = false;
    void resetPaintFlags();

    GraphicItemTerm* getNodeAtPoint(const QPointF &pt) const;
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

    // Tools
    void hideRect(QGraphicsRectItem* item);

    // Main objects
    TGroupName *grNmItem;
    void addTreeRectsToScene();
    QGraphicsRectItem *orphansRect;
};

#endif  // TERMGROUP_H
