#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QMessageBox>
#include <QGraphicsView>

#include "glb.h"
#include "termgroup.h"
#include "mainwindow.h"

class MainScene : public QGraphicsScene
{
    Q_OBJECT

    NodesList allNodesList;
    EdgesList allEdgesList;

    void initGroups();
    void destructGroups();
    void formAllNodeList();
    void appendEdgesToScene();

    QTimer selectTimer;
//    QTimer viewGrpTimer;
//    int    timerCount;
    QTimer sceneRhytm;

    int currGroupIndex = 0;

public:
    qreal xWindow;
    qreal yWindow;//dirtyHack

    NodesList getAllNodes();

    QList < TermGroup* > groupList;

    TermGroup* getGroup(QString name);

    static DBAbstract *db;

    MainScene();
    ~MainScene();

    void mouseMoveEvent(QGraphicsSceneMouseEvent *evt);
    void mousePressEvent(QGraphicsSceneMouseEvent *evt);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *evt);

    TermNode* getSelected();
    TermNode* getNodeByUid(int uid);
    NodesList getAllTermsAtPoint  ( int x, int y );

    void setAnimSpeed(int val);

signals:
    //    void mousePos(QString);

    void stopMove();

    void showMessage(QString msg,int time);
    void updateGroupLists();

    void someSelected();
    void selectionDrop();

    void mouseInfo(QString str);

public slots:
    QStringList getGroupsNames();

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

    void dropEdgesOnSelected();

    void updateGroupsGeometry();
    void locateGroupsVertically();

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

    void addNewGroup(QString name, QString comment, int type);
    void deleteGroup(QString name);
    void exportGrpToJson(QString grpName);

    void addNewNode(
            QString name,
            QString forms,
            QString def,
            QString descr,
            QString exam,
            QString groupName
                    );

    void changeNode(
            QString nodeUuid,
            QString name,
            QString forms,
            QString def,
            QString descr,
            QString exam,
            QString groupName
                    );

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
    void stayGroupInFocus( bool val ) { groupInFocus = val; }

    void createTestGroups();
};

#endif // MAINSCENE_H
