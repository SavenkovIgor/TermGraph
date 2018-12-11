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

class MainScene : public QGraphicsScene  // TODO: Maybe remove QGraphicScene dependency
{
    Q_OBJECT
    Q_PROPERTY(QRectF rect READ getSceneRect)

    void initAllGroups();
    void addGroupToScene(TermGroup* group);
    void deleteAllGroups();

    QTimer sceneRhytm;

    QTimer userInactiveTimer;
    QTimer mouseMoveReactionTimer;

    int currGroupIndex = 0;

public:
    MainScene(GroupsManager* groupsMgr, NodesManager* nodesMgr);
    ~MainScene();

    PaintManager* getPaintManager();

    TermGroup* getGroupByName(QString name);
    TermGroup* getGroupByUuid(QUuid uuid);

    GraphicItemTerm::List getAllNodes();

    GraphicItemTerm* getSelected();

    void setAnimSpeed(int val);

signals:
    void someSelected();
    void selectionDrop();

    void sceneUpdated();

    void showInfo(QString info);
    void showWarning(QString warning);
    void showError(QString error);

public slots:
    void showGroup(QString groupUuid);
    void showGroup(QUuid groupUuid);
    void showAllGroups();

    void locateGroupsVertically();
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

public slots:
    bool hasSelection();

    void exportGrpToJson(QString groupName);
    void saveGroupInFolder(TermGroup *group);
    QString getExportPath();

    QString getGroupString(QString grp);

    QStringList getGroupTypes() {
        return TermGroup::getTypesNames();
    }

private:
    // Timer
    QElapsedTimer elTimer;

public slots:

    // Drawing API
    // Timer
    void startCheckTimer();
    void restartCheckTimer(QString label = "paint speed");

    // ViewFrame
    void setSceneViewRect(int x, int y, int width, int height);

    QColor getSceneBackgroundColor();

    void resetPaintFlags();

    void setMousePos(qreal x, qreal y);
    void setMouseClick(qreal x, qreal y);

    // For testing
    void createTestGroups();

private:
    GroupsManager* groupsMgr;
    NodesManager* nodesMgr;

    QRectF sceneRect = QRectF(0, 0, 100, 100);
    QRectF getSceneRect() const;

    QRectF sceneViewRect = QRectF(0, 0, 100, 100);

    // Mouse interaction
    GraphicItemTerm* hoverNode = nullptr;
    GraphicItemTerm* selectedNode = nullptr;

    GraphicItemTerm* getNodeAtPoint(const QPointF& pt) const;

    void findHover(const QPointF& atPt);
    void findClick(const QPointF& atPt);

    PaintManager* paintManager;

    QList < TermGroup* > groupList;
    QUuid currGroupUuid;

    QString getCurrNodeStringField(std::function<QString (InfoTerm*)> strFunction);
};

#endif  // MAINSCENE_H
