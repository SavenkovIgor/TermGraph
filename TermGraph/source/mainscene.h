#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <stdlib.h>

#include <QObject>
#include <QClipboard>
#include <QElapsedTimer>

#include "source/paintqueuemanager.h"
#include "source/Managers/groupsmanager.h"
#include "source/Managers/nodesmanager.h"
#include "source/Helpers/appconfig.h"
#include "source/Helpers/fsworks.h"
#include "source/Model/TerminGroup/termgroup.h"

class MainScene : public QObject
{
    Q_OBJECT

public:
    MainScene(GroupsManager* groupsMgr, NodesManager* nodesMgr, PaintManager* paintManager);
    ~MainScene();

    void setAnimSpeed(int val);

    Q_PROPERTY(bool hasSelection READ isAnyNodeSelected NOTIFY selectionChanged)

signals:
    // Scene signals
    void selectionChanged();
    void selectionDoubleClick();
    void sceneContentUpdated();

    // Debug signals
    void showInfo(QString info);
    void showWarning(QString warning);
    void showError(QString error);

public slots:
    void showGroup(const QString& groupUuid);
    void showGroup(const QUuid& groupUuid);
    void showAllGroups();

    void updateSceneRect();
    void centerViewOn(QPointF point);  // TODO: Realize!

    void updateModel();
    void deleteSelectedNode();

    void startAllGroupTimers();
    void stopAllGroupTimers();

    QString getCurrNodeDebugInfo();

    QString getCurrNodeUuid();
    QString getCurrNodeName();
    QString getCurrNodeNameAndDefinition();
    QString getCurrNodeForms();
    QString getCurrNodeDefinition();
    QString getCurrNodeHierarchyDefinition();
    QString getCurrNodeDescription();
    QString getCurrNodeExamples();
    QString getCurrNodeGroupUuid();
    QString getCurrGroupUuid();
    bool    getCurrNodeIsRoot();

    TermGroup* getNearestNotPaintedGroup();

    bool isAnyNodeSelected();

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
    QTimer mouseMoveReactionTimer;

    // Managers
    GroupsManager* groupsMgr;
    NodesManager* nodesMgr;
    PaintManager* paintManager;

    void requestPaint(bool paintAll = false);
    void sendGroupsToPaintManager(bool requestPaint = true, bool paintAll = false);

    // Scene rect
    QRectF sceneRect = QRectF(0, 0, 100, 100);
    QRectF sceneViewRect = QRectF(0, 0, 100, 100);

    // Mouse interaction
    PaintedTerm* hoverNode = nullptr;
    PaintedTerm* selectedNode = nullptr;

    PaintedTerm* getSelectedNode();
    void dropSelection(bool withSignal = true);
    void dropHover();

    void findHover(const QPointF& atPt);
    void findClick(const QPointF& atPt);

    PaintedTerm* getNodeAtPoint(const QPointF& pt) const;

    // Groups fields
    QList<TermGroup*> groupList;
    QUuid currGroupUuid;

    void initAllGroups();
    void addGroupToScene(TermGroup* group);
    void deleteAllGroups();

    void locateGroupsVertically();

    // Helpers
    QString getCurrNodeStringField(std::function<QString (InfoTerm*)> strFunction);
};

#endif  // MAINSCENE_H
