#include "mainscene.h"

MainScene::MainScene(GroupsManager* groupsMgr, NodesManager* nodesMgr) : QGraphicsScene()
{
    sceneRhytm.setSingleShot(false);
    sceneRhytm.setInterval(30);

    selectTimer.setSingleShot(false);
    selectTimer.setInterval(200);
    connect(&selectTimer, SIGNAL(timeout()), SLOT(checkSelection()));

//    viewGrpTimer.setSingleShot(false);
//    connect(&viewGrpTimer,SIGNAL(timeout()),SLOT(showGroup()));

    if (groupsMgr == nullptr) {
        qDebug() << "Critical error: groupsManager is null!";
    }

    if (nodesMgr == nullptr) {
        qDebug() << "Critical error: nodesManager is null!";
    }

    this->groupsMgr = groupsMgr;
    connect(groupsMgr, SIGNAL(groupsListChanged()), SLOT(updateModel()));

    this->nodesMgr = nodesMgr;
    connect(nodesMgr, SIGNAL(nodeChanged()), SLOT(updateModel()));

    setItemIndexMethod(NoIndex);
    updateModel();
}

MainScene::~MainScene()
{
}

void MainScene::initAllGroups()
{
    for (TermGroup* group : groupsMgr->getAllGroups()) {
        addGroupToScene(group);
    }
}

void MainScene::addGroupToScene(TermGroup *group)
{
    addItem(group->baseRect);
    connect(&sceneRhytm, SIGNAL(timeout()), group, SLOT(sceneUpdateSignal()));
    groupList << group;
}

void MainScene::deleteAllGroups()
{
    for (TermGroup* group : groupList) {
        removeItem(group->baseRect);
    }

    groupList.clear();
}

NodesList MainScene::getAllNodes()
{
    NodesList ret;
    for (TermGroup* group : groupList) {
        ret << group->getAllNodes();
    }
    return ret;
}

TermGroup *MainScene::getGroupByName(QString name)
{
    for (TermGroup* group : groupList) {
        if (group->getName() == name) {
            return group;
        }
    }

    return nullptr;
}

TermGroup *MainScene::getGroupByUuid(QUuid uuid)
{
    for (TermGroup* group : groupList) {
        if (group->getUuid() == uuid) {
            return group;
        }
    }

    return nullptr;
}

void MainScene::mouseMoveEvent(QGraphicsSceneMouseEvent *evt)
{
    mouseInfo("move");
    evt->setScreenPos(evt->screenPos() - QPointF(xWindow, yWindow).toPoint());

    QGraphicsScene::mouseMoveEvent(evt);
}

void MainScene::mousePressEvent(QGraphicsSceneMouseEvent *evt)
{
    //    qDebug()<<"press";
    // setScenePos влияет на позицию курсора в сцене но только во время
    // соотв. события (нажатия клавиши и пр). довольно тупо пытаться
    // это так использовать
    // TODO: Переписать! вынести xWindow в вид по возможности
    evt->setScreenPos(evt->screenPos() - QPointF(xWindow, yWindow).toPoint());
//    evt->setScenePos(evt->scenePos() + QPoint(15,yWindow));

//    mouseInfo( "press at " + Glb::ptToStr(evt->scenePos()) );
    lastPressPt = evt->scenePos();

    QGraphicsScene::mousePressEvent(evt);
}

void MainScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *evt)
{
    //    qDebug()<<"release";
    mouseInfo("release");
    evt->setScreenPos(evt->screenPos() - QPointF(xWindow, yWindow).toPoint());
    NodesList nodesList = getAllTermsAtPoint(evt->scenePos());
    if (nodesList.size() == 1) {
        int dist = 0;
        dist += qAbs(lastPressPt.x() - evt->scenePos().x());
        dist += qAbs(lastPressPt.y() - evt->scenePos().y());

        if (dist <= 30) {
            qDebug() << dist << lastPressPt << evt->scenePos();
            nodesList.first()->setFlag(QGraphicsItem::ItemIsSelectable, true);
            nodesList.first()->setSelected(true);
            evt->accept();
        }
    } else {
        for (TermNode* node : getAllNodes()) {
            node->setFlag(QGraphicsItem::ItemIsSelectable, false);
            node->setSelected(false);
            //                    evt->accept();
        }
    }

    QGraphicsScene::mouseReleaseEvent(evt);
}

void MainScene::updateModel()
{
    selectTimer.stop();
//    viewGrpTimer.stop();
    sceneRhytm.stop();
    stopAllGroupTimers();

    deleteAllGroups();
    initAllGroups();

    for (TermGroup* group : groupList) {
        group->sceneUpdateSignal();
    }

    locateGroupsVertically();

    selectTimer.start();
//    viewGrpTimer.start(200);
    sceneRhytm.start();
    startAllGroupTimers();
}

void MainScene::locateGroupsVertically()
{
    int y = 0;

    // Выставляем позиции групп
    for (TermGroup* group : groupList) {
        QRectF baseRc = group->baseRect->rect().translated(group->baseRect->scenePos());
        group->setBasePoint(QPointF(0, y));

        y += baseRc.height() + 40;
    }

    updateSceneRect();
}

void MainScene::updateSceneRect()
{
    QRectF allRect;

    for (TermGroup* group : groupList) {
        if (!group->baseRect->isVisible()) {
            continue;
        }

        QRectF baseRc = group->baseRect->rect().translated(group->baseRect->scenePos());
        allRect = allRect.united(baseRc);
    }

    int mV = 50;
    QMarginsF mrg(mV, mV, mV, mV);
    setSceneRect(allRect.marginsAdded(mrg));
}

void MainScene::centerViewOn(QPointF point)
{
    QList<QGraphicsView*> viewsList = views();

    for (QGraphicsView* view : viewsList) {
        view->centerOn(point);
    }
}

void MainScene::deleteSelectedNode()
{
    TermNode* node = getSelected();
    if (node == nullptr) {
        return;
    }

    nodesMgr->deleteNode(node->getUuid());
}

void MainScene::exportGrpToJson(QString groupName)
{
    TermGroup* group = getGroupByName(groupName);
    if (group == nullptr) {
        return;
    }

    saveGroupInFolder(group);

    QJsonDocument document = group->getJsonDoc();
    QClipboard* clipboard = qApp->clipboard();
    clipboard->setText(document.toJson());
}

void MainScene::saveGroupInFolder(TermGroup* group)
{
    if (group == nullptr) {
        return;
    }

    QString fileName = group->getName() + " " + group->getUuid().toString() + ".grp";
    FSWorks::saveFile(AppConfig::StdFolderPaths::groupsJsonFolder(), fileName, group->getJsonDoc().toJson());
}

QString MainScene::getExportPath()
{
    return AppConfig::StdFolderPaths::groupsJsonFolder();
}

QString MainScene::getGroupString(QString grp)
{
    TermGroup* group = getGroupByName(grp);
    if (group != nullptr) {
        return group->getTypeString();
    }

    return "";
}

void MainScene::showGroup(int num)
{
    QStringList allGroupNames = groupsMgr->getAllGroupsNames();

    if (num < 0 || num >= allGroupNames.size()) {
        return;
    }

    showGroup(allGroupNames[num]);
}

TermNode *MainScene::getSelected()
{
    TermNode* ret = nullptr;

    QList<QGraphicsItem*> sel = selectedItems();
    if (sel.size() != 1) {
        return ret;
    }

    ret = dynamic_cast<TermNode*>(sel.first());
    return ret;
}

NodesList MainScene::getAllTermsAtPoint(QPointF point) {
    NodesList ret;
    for (TermNode* node : getAllNodes()) {
        if (node->getNodeRect(CoordType::scene).contains(point)) {
            ret << node;
        }
    }
    return ret;
}

void MainScene::showGroup(QString groupName)
{
//    if( viewGrpTimer.isActive() ) {
//        timerCount++;
//        if( timerCount > 5 ) {
//            viewGrpTimer.stop();
//            timerCount = 0;
//        }
//    }

    static QString lastGroupName;
    if (groupName == "") {
        groupName = lastGroupName;
    }

    if (groupName == "Все группы") {
        for (TermGroup* group : groupList) {
            group->baseRect->show();
        }

    } else {
        for (TermGroup* group : groupList) {
            if (group->getName().contains(groupName)) {
                group->baseRect->show();
                QRectF baseRc = group->baseRect->rect().translated(group->baseRect->scenePos());
                centerViewOn(baseRc.center());
            } else {
                group->baseRect->hide();
            }
        }
    }

    updateSceneRect();

    lastGroupName = groupName;
}

void MainScene::setAnimSpeed(int val)
{
    for (TermGroup* group : groupList) {
        group->setAnimSpeed(val);
    }
}

void MainScene::checkSelection()
{
    bool someSel = false;
    for (TermNode* node : getAllNodes()) {
        if (node->isSelected()) {
            someSelected();
            someSel = true;
            TermNode::someoneSelect = true;
            node->setRelatPaint(true);
        }
    }

    if (!someSel) {
        selectionDrop();
        TermNode::someoneSelect = false;
        if (!TermNode::someoneHover) {
            for (TermNode* node : getAllNodes()) {
                node->setRelatPaint(false);
            }
        }
    }

//    qDebug()<<"count of items"<<this->items().count();
}

void MainScene::startAllGroupTimers()
{
    for (TermGroup* group : groupList)
        group->startAnimation();
}

void MainScene::stopAllGroupTimers()
{
    for (TermGroup* group : groupList)
        group->stopAnimation();
}

QString MainScene::getCurrNodeDebugInfo()
{
    return "";
//    TermNode* nd = getSelected();
//    if (nd == nullptr) {
//        return "";
//    }

//    QStringList ret;
//    ret << "Uuid:" << nd->getUuid().toString();
//    ret << nd->getDebugString();

//    return ret.join(" ");
}

QString MainScene::getCurrNodeLongUid()
{
    if (TermNode* node = getSelected()) {
        return node->getUuid().toString();
    }
    return "";
}

QString MainScene::getCurrNodeName()
{
    if (TermNode* node = getSelected()) {
        return node->getName();
    }
    return "";
}

QString MainScene::getCurrNodeForms()
{
    if (TermNode* node = getSelected()) {
        return node->getNameFormStr();
    }
    return "";
}

QString MainScene::getCurrNodeDefinition()
{
    if (TermNode* node = getSelected()) {
        return node->getDefinition();
    }
    return "";
}

QString MainScene::getCurrNodeDescription()
{
    if (TermNode* node = getSelected()) {
        return node->getDescription();
    }
    return "";
}

QString MainScene::getCurrNodeExamples()
{
    if (TermNode* node = getSelected()) {
        return node->getExamples();
    }
    return "";
}

QString MainScene::getCurrNodeGroupName()
{
    if (TermNode* node = getSelected()) {
        QUuid uuid = node->getGroupUuid();
        if (!uuid.isNull()) {
            return groupsMgr->getGroupName(uuid);
        }
    }
    return "";
}

bool MainScene::getCurrNodeIsRoot()
{
    TermNode* node = getSelected();
    if (node == nullptr) {
        return false;
    }

    return node->isRoot();
    // TODO: Возможно переделать в функциональном стиле!
}

void MainScene::createTestGroups()
{
    groupsMgr->addNewGroup("TestGroup1", "", GroupType::terms);

    nodesMgr->addNewNode("1", "", "", "", "", "TestGroup1");
    nodesMgr->addNewNode("2", "", "", "", "", "TestGroup1");
    nodesMgr->addNewNode("3", "", "{1}{2}", "", "", "TestGroup1");
    nodesMgr->addNewNode("4", "", "{1}{2}", "", "", "TestGroup1");
    nodesMgr->addNewNode("5", "", "{1}", "", "", "TestGroup1");
    nodesMgr->addNewNode("6", "", "{5}", "", "", "TestGroup1");
    nodesMgr->addNewNode("7", "", "{5}", "", "", "TestGroup1");
    nodesMgr->addNewNode("8", "", "{6}{7}", "", "", "TestGroup1");
    nodesMgr->addNewNode("9", "", "", "", "", "TestGroup1");
}
