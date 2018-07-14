#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QMessageBox>
#include <QGraphicsView>

#include "glb.h"
#include "Managers/networkmanager.h"
#include "Managers/groupsmanager.h"
#include "Managers/nodesmanager.h"
#include "termgroup.h"
#include "mainwindow.h"

class MainScene : public QGraphicsScene
{
    Q_OBJECT

    TermGroup* createGroupFromJson(QJsonDocument json);
    bool isValidGroupJson(QJsonDocument json);

    void initAllGroups();
    void addGroupToScene(TermGroup* group);
    void deleteAllGroups();

    QTimer selectTimer;
//    QTimer viewGrpTimer;
//    int    timerCount;
    QTimer sceneRhytm;

    int currGroupIndex = 0;

public:
    MainScene();
    ~MainScene();

    qreal xWindow;
    qreal yWindow;//dirtyHack

    QList < TermGroup* > groupList;
    TermGroup* getGroupByName(QString name);
    TermGroup* getGroupByUuid(QUuid uuid);

    NodesList getAllNodes();

    static DBAbstract *db; //TODO: Сделать инжект! или избавиться в идеале

    void mouseMoveEvent(QGraphicsSceneMouseEvent *evt);
    void mousePressEvent(QGraphicsSceneMouseEvent *evt);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *evt);

    TermNode* getSelected();
    NodesList getAllTermsAtPoint  ( int x, int y );

    void setAnimSpeed(int val);

signals:
    //    void mousePos(QString);

    void stopMove();

    void showMessage(QString msg,int time);

    void someSelected();
    void selectionDrop();

    void mouseInfo(QString str);

public slots:
    QStringList getGroupsNames(bool withAllVeiw = false);

    void showGroup(int num); //TODO: Постараться избавиться от этой функции
    void showGroup( QString grp = "");

    void toPreviousGroup() {
        currGroupIndex = qBound( 0, currGroupIndex - 1, groupList.size() - 1 );
        showGroup(currGroupIndex);
    }

    void toNextGroup() {
        currGroupIndex = qBound( 0, currGroupIndex + 1, groupList.size() - 1 );
        showGroup(currGroupIndex);
    }

    void locateGroupsVertically();
    void updateSceneRect();
    void centerViewOn(QPointF pt);

    void updateModel();
    void dropSelectedNode();

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
    bool    getCurrNodeIsRoot();

    bool hasSelection(){
        return getSelected() != nullptr;
    }

    void exportGrpToJson(QString grpName);
    void saveGroupInFolder(TermGroup *g);
    QString getExportPath();

    void addNewNode(
            QString name,
            QString forms,
            QString def,
            QString descr,
            QString exam,
            QString groupName
                    );

    void changeNode(
            QUuid nodeUuid,
            QString name,
            QString forms,
            QString def,
            QString descr,
            QString exam,
            QString groupName
                    );

    void importFile(QString filename);

    QString getGroupString(QString grp);

//    void testSlot() {
//        groupList[0]->setBasePoint(QPoint(200,200));
//        sceneRhytm.stop();
//    }

    QStringList getGroupTypes() {
        return TermGroup::getTypesNames();
    }

    void sendGroupByNetwork(QString groupName);
    void setReceiverHost(QString ip);

    // For registering in qml
    NetworkManager* getNetworkManager() { return network; }
    GroupsManager* getGroupsManager() { return groupsMgr; }
    NodesManager* getNodesManager() { return nodesMgr; }

private:
    QPointF lastPressPt;
    bool groupInFocus = false;

    QString receiverIp = "127.0.0.1";
public slots:
    void stayGroupInFocus( bool val ) { groupInFocus = val; }
    void createTestGroups();

private slots:
    void importGroupFromJson(QString rawJson);
    void importGroupFromJson(QJsonDocument json);

private:
    NetworkManager* network; //TODO: Сделать инжект!
    GroupsManager* groupsMgr; //TODO: Сделать инжект!
    NodesManager* nodesMgr; //TODO: Сделать инжект!
};

#endif // MAINSCENE_H
