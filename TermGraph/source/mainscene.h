#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <stdlib.h>

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QClipboard>
#include <QElapsedTimer>

#include "./paintqueuemanager.h"
#include "./Managers/groupsmanager.h"
#include "./Managers/nodesmanager.h"
#include "./Helpers/appconfig.h"
#include "./Helpers/fsworks.h"
#include "./Model/TerminGroup/termgroup.h"

class MainScene : public QGraphicsScene  // NOTE: Maybe remove QGraphicScene dependency
{
    Q_OBJECT

public:
    MainScene(GroupsManager* groupsMgr, NodesManager* nodesMgr, PaintManager* paintManager);
    ~MainScene();

    void setAnimSpeed(int val);

signals:
    // Scene signals
    void someSelected();
    void selectionDrop();
    void sceneContentUpdated();

    // Debug signals
    void showInfo(QString info);
    void showWarning(QString warning);
    void showError(QString error);

public slots:
    void showGroup(QString groupUuid);
    void showGroup(QUuid groupUuid);
    void showAllGroups();

    void updateSceneRect();
    void centerViewOn(QPointF point);  // TODO: Realize!

    void updateModel();
    void deleteSelectedNode();

    void startAllGroupTimers();
    void stopAllGroupTimers();

    QString getCurrNodeDebugInfo();

    QString getCurrNodeLongUid();
    QString getCurrNodeName();
    QString getCurrNodeForms();
    QString getCurrNodeDefinition();
    QString getCurrNodeDescription();
    QString getCurrNodeExamples();
    QString getCurrNodeGroupName();
    bool    getCurrNodeIsRoot();

    TermGroup* getNearestNotPaintedGroup();
    void paintOneGroupIfNeed();

    bool hasSelection();

    // Drawing API
    // ViewFrame
    void setSceneViewRect(int x, int y, int width, int height);

    QRectF getSceneRect() const;
    QColor getSceneBackgroundColor() const;

    void resetPaintFlags();

    void setMousePos(qreal x, qreal y);
    void setMouseClick(qreal x, qreal y);

    // For testing
    void createTestGroups();

private:
    // Timers
    QTimer sceneRhytm;
    QTimer userInactiveTimer;
    QTimer mouseMoveReactionTimer;

    // Managers
    GroupsManager* groupsMgr;
    NodesManager* nodesMgr;
    PaintManager* paintManager;

    // Scene rect
    QRectF sceneRect = QRectF(0, 0, 100, 100);
    QRectF sceneViewRect = QRectF(0, 0, 100, 100);

    // Mouse interaction
    GraphicItemTerm* hoverNode = nullptr;
    GraphicItemTerm* selectedNode = nullptr;

    GraphicItemTerm* getSelectedNode();

    void findHover(const QPointF& atPt);
    void findClick(const QPointF& atPt);

    GraphicItemTerm* getNodeAtPoint(const QPointF& pt) const;

    // Groups fields
    QList < TermGroup* > groupList;
    QUuid currGroupUuid;

    void initAllGroups();
    void addGroupToScene(TermGroup* group);
    void deleteAllGroups();

    void locateGroupsVertically();

    // Helpers
    QString getCurrNodeStringField(std::function<QString (InfoTerm*)> strFunction);
};

#endif  // MAINSCENE_H
