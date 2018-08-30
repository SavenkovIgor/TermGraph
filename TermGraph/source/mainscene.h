#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QClipboard>

#include "./glb.h"
#include "./Managers/groupsmanager.h"
#include "./Managers/nodesmanager.h"
#include "./termgroup.h"

class MainScene : public QGraphicsScene
{
    Q_OBJECT

    void initAllGroups();
    void addGroupToScene(TermGroup* group);
    void deleteAllGroups();

    QTimer selectTimer;
//    QTimer viewGrpTimer;
//    int    timerCount;
    QTimer sceneRhytm;

    int currGroupIndex = 0;

public:
    MainScene(GroupsManager* groupsMgr, NodesManager* nodesMgr);
    ~MainScene();

    qreal xWindow;
    qreal yWindow;  // dirtyHack

    QList < TermGroup* > groupList;
    TermGroup* getGroupByName(QString name);
    TermGroup* getGroupByUuid(QUuid uuid);

    NodesList getAllNodes();

    void mouseMoveEvent(QGraphicsSceneMouseEvent *evt);
    void mousePressEvent(QGraphicsSceneMouseEvent *evt);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *evt);

    TermNode* getSelected();

    void setAnimSpeed(int val);

signals:
    //    void mousePos(QString);

    void stopMove();

    void showMessage(QString msg, int time);

    void someSelected();
    void selectionDrop();

    void mouseInfo(QString str);

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

    bool hasSelection(){
        return getSelected() != nullptr;
    }

    void exportGrpToJson(QString groupName);
    void saveGroupInFolder(TermGroup *group);
    QString getExportPath();

    QString getGroupString(QString grp);

//    void testSlot() {
//        groupList[0]->setBasePoint(QPoint(200,200));
//        sceneRhytm.stop();
//    }

    QStringList getGroupTypes() {
        return TermGroup::getTypesNames();
    }

private:
    QPointF lastPressPt;
    bool groupInFocus = false;

public slots:
    void stayGroupInFocus(bool val) { groupInFocus = val; }
    void createTestGroups();

private:
    GroupsManager* groupsMgr;
    NodesManager* nodesMgr;

    NodesList getAllTermsAtPoint(QPointF point);
};

#endif  // MAINSCENE_H
