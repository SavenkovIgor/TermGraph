#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <stdlib.h>

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QClipboard>
#include <QElapsedTimer>

#include "./Managers/groupsmanager.h"
#include "./Managers/nodesmanager.h"
#include "./Helpers/appconfig.h"
#include "./Helpers/fsworks.h"
#include "./Model/TerminGroup/termgroup.h"

class MainScene : public QGraphicsScene
{
    Q_OBJECT
    Q_PROPERTY(QRectF rect READ getSceneRect)

    void initAllGroups();
    void addGroupToScene(TermGroup* group);
    void deleteAllGroups();

    QTimer selectTimer;
//    QTimer viewGrpTimer;
//    int    timerCount;
    QTimer sceneRhytm;

    QTimer userInactiveTimer;
    QTimer mouseMoveReactionTimer;

    int currGroupIndex = 0;

public:
    MainScene(GroupsManager* groupsMgr, NodesManager* nodesMgr);
    ~MainScene();

    qreal xWindow;
    qreal yWindow;  // dirtyHack

    TermGroup* getGroupByName(QString name);
    TermGroup* getGroupByUuid(QUuid uuid);

    GraphicItemTerm::List getAllNodes();

    void mouseMoveEvent(QGraphicsSceneMouseEvent *evt);
    void mousePressEvent(QGraphicsSceneMouseEvent *evt);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *evt);

    GraphicItemTerm* getSelected();

    void setAnimSpeed(int val);

signals:
    //    void mousePos(QString);

    void stopMove();

    void someSelected();
    void selectionDrop();

    void mouseInfo(QString str);

    void sceneUpdated();

    void repaintQmlScene();

    void showInfo(QString info);
    void showWarning(QString warning);
    void showError(QString error);

public slots:
    void showGroup(int num);  // TODO: Постараться избавиться от этой функции
    void showGroup(QString grp = "");

    void toPreviousGroup() {
        currGroupIndex = qBound(0, currGroupIndex - 1, groupList.size() - 1);
        showGroup(currGroupIndex);
    }

    void toNextGroup() {
        currGroupIndex = qBound(0, currGroupIndex + 1, groupList.size() - 1);
        showGroup(currGroupIndex);
    }

    void locateGroupsVertically();
    void updateSceneRect();
    void centerViewOn(QPointF point);

    void updateModel();
    void deleteSelectedNode();

    void checkSelection();

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
private:
    QString getCurrNodeStringField(std::function<QString (InfoTerm*)> strFunction);

public slots:
    bool hasSelection(){
        return getSelected() != nullptr;
    }

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

    // Group iterator
    void startGroupIterator();
    void nextGroup();
    bool groupIteratorAtEnd();

    QRectF currentGroupRect();
    QColor currentGroupFillColor();
    QString currentGroupName();
    QPointF currentGroupNamePos();
    QStringList currentGroupAllNodeNames();
    QList<QRectF> currentGroupAllRects();

    // Edge iterator
    void startEdgeIterator();
    void nextEdge();
    bool edgeIteratorAtEnd();
    QColor getEdgeColor();

    QPointF currentFirstEdgePoint();
    QPointF currentLastEdgePoint();

    // Node iterator
    void startNodeIterator();
    void nextNode();
    bool nodeIteratorAtEnd();

    QRectF currentNodeRect();
    QPointF currentNodeCenter();
    QColor currentNodeColor();
    QStringList currentNodeText();
    qreal currentNodeRadius();

    QColor getSceneBackgroundColor();

    void resetPaintFlags();

    void setPaintInProcess(bool painting);

    void setMousePos(qreal x, qreal y);

private:
    int groupIterator = 0;
    int edgeIterator = 0;
    int nodeIterator = 0;


private:
    QPointF lastPressPt;
    bool groupInFocus = false;

public slots:
    void stayGroupInFocus(bool val) { groupInFocus = val; }
    void createTestGroups();

private:
    GroupsManager* groupsMgr;
    NodesManager* nodesMgr;

    QRectF sceneRect = QRectF(0, 0, 100, 100);
    QRectF getSceneRect();

    QRectF sceneViewRect = QRectF(0, 0, 100, 100);

    bool paintInProcess = false;

    QPointF mousePos;
    void findSelection();

    QList < TermGroup* > groupList;
    QList < TermGroup* > paintQueueGroupList;

    GraphicItemTerm::List getAllTermsAtPoint(QPointF point);
};

#endif  // MAINSCENE_H
