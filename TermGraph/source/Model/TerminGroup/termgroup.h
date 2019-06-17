#ifndef TERMGROUP_H
#define TERMGROUP_H

#include <QObject>
#include <QtSql>

#include "../../Helpers/appstyle.h"
#include "../../Helpers/tagprocessor.h"
#include "../GraphicItem/rectgraphicitem.h"
#include "./termgroupinfo.h"
#include "../TerminEdge/edge.h"
#include "../GraphicItem/labelgraphicitem.h"

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

    RectGraphicItem *baseRect;

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

    // Position and frame
    void setBasePoint(QPointF pt);
    void updateGroupFrame();

    QJsonDocument getJsonDoc();

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

#endif  // TERMGROUP_H
