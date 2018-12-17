#include "mainscene.h"

MainScene::MainScene(GroupsManager* groupsMgr, NodesManager* nodesMgr) : QGraphicsScene()
{
    sceneRhytm.setSingleShot(false);
    sceneRhytm.setInterval(30);

    userInactiveTimer.setInterval(static_cast<int>(1000/AppConfig::SceneSettings::FPS));
    userInactiveTimer.setSingleShot(true);
    connect(&userInactiveTimer, SIGNAL(timeout()), SLOT(paintOneGroupIfNeed()));

    mouseMoveReactionTimer.setInterval(static_cast<int>(1000/AppConfig::SceneSettings::FPS));
    mouseMoveReactionTimer.setSingleShot(true);

    Q_ASSERT(groupsMgr != nullptr);
    this->groupsMgr = groupsMgr;
    connect(groupsMgr, SIGNAL(groupsListChanged()), SLOT(updateModel()));

    Q_ASSERT(nodesMgr != nullptr);
    this->nodesMgr = nodesMgr;
    connect(nodesMgr, SIGNAL(nodeChanged()), SLOT(updateModel()));

    paintManager = new PaintManager();

    setItemIndexMethod(NoIndex);
    updateModel();
}

MainScene::~MainScene()
{
    delete paintManager;
}

void MainScene::initAllGroups()
{
    QUuid loadGroup = currGroupUuid;

    if (loadGroup.isNull()) {
        auto allGroupsUuids = groupsMgr->getAllUuidsSortedByLastEdit();
        if (!allGroupsUuids.isEmpty()) {
            loadGroup = allGroupsUuids.first();
        }
    }

    if (!loadGroup.isNull()) {
        auto group = groupsMgr->createGroup(loadGroup);
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
    for (auto group : groupList) {
        removeItem(group->baseRect);
    }

    groupList.clear();
}

GraphicItemTerm::List MainScene::getAllNodes()
{
    GraphicItemTerm::List ret;
    for (auto group : groupList) {
        ret << group->getAllNodes();
    }
    return ret;
}

TermGroup *MainScene::getGroupByName(QString name)
{
    for (auto group : groupList) {
        if (group->getName() == name) {
            return group;
        }
    }

    return nullptr;
}

void MainScene::updateModel()
{
    sceneRhytm.stop();
    userInactiveTimer.stop();
    stopAllGroupTimers();

    paintManager->sendCleanAllSignal();
    deleteAllGroups();
    initAllGroups();

    for (auto group : groupList) {
        group->sceneUpdateSignal();
    }

    locateGroupsVertically();

    sceneRhytm.start();
    // startAllGroupTimers();
    updateSceneRect();
    sceneContentUpdated();
    userInactiveTimer.start();
    qDebug() << "model updated";
}

void MainScene::locateGroupsVertically()
{
    auto y = 40;
    auto x = 40;

    // Выставляем позиции групп
    for (TermGroup* group : groupList) {
        QRectF baseRc = group->baseRect->rect().translated(group->baseRect->scenePos());
        group->setBasePoint(QPointF(x, y));

        y += baseRc.height() + 40;
    }

    updateSceneRect();
}

void MainScene::updateSceneRect()
{
    QRectF allRect;

    for (auto group : groupList) {
        if (group->getUuid() != currGroupUuid) {
            continue;
        }

        QRectF baseRc = group->baseRect->rect().translated(group->baseRect->scenePos());
        allRect = allRect.united(baseRc);
    }

    int mV = 40;
    QMarginsF mrg(mV, mV, mV, mV);
    auto withMargins = allRect.marginsAdded(mrg);
    sceneRect = withMargins;
    sceneRect = sceneRect.united(sceneViewRect);
    setSceneRect(sceneRect);
}

void MainScene::centerViewOn(QPointF point)
{

}

void MainScene::deleteSelectedNode()
{
    if (auto node = getSelectedNode()) {
        nodesMgr->deleteNode(node->getUuid());
    }
}

void MainScene::exportGrpToJson(QString groupName)
{
    if (auto group = getGroupByName(groupName)) {

        saveGroupInFolder(group);

        QJsonDocument document = group->getJsonDoc();
        QClipboard* clipboard = qApp->clipboard();
        clipboard->setText(document.toJson());
    }
}

void MainScene::saveGroupInFolder(TermGroup* group)
{
    if (group != nullptr) {
        QString fileName = group->getName() + " " + group->getUuid().toString() + ".grp";
        FSWorks::saveFile(AppConfig::StdFolderPaths::groupsJsonFolder(), fileName, group->getJsonDoc().toJson());
    }
}

QString MainScene::getExportPath()
{
    return AppConfig::StdFolderPaths::groupsJsonFolder();
}

QString MainScene::getGroupString(QString grp)
{
    if (auto group = getGroupByName(grp)) {
        return group->getTypeString();
    }

    return "";
}

void MainScene::setSceneViewRect(int x, int y, int width, int height)
{
    sceneViewRect = QRectF(x, y, width, height);

    auto tmpRect = sceneRect;
    tmpRect.setWidth(qMax(sceneRect.width(), sceneViewRect.width()));
    tmpRect.setHeight(qMax(sceneRect.height(), sceneViewRect.height()));

    if (tmpRect != sceneRect) {
        sceneRect = tmpRect;
//        resetPaintFlags();
//        sceneContentUpdated();
    }

    userInactiveTimer.start();
}

QColor MainScene::getSceneBackgroundColor()
{
    return AppStyle::Colors::Scene::background;
}

void MainScene::resetPaintFlags()
{
    for (auto group : groupList) {
        group->resetPaintFlags();
    }
}

void MainScene::setMousePos(qreal x, qreal y)
{
    findHover(QPointF(x,y));
}

void MainScene::setMouseClick(qreal x, qreal y)
{
    findClick(QPointF(x,y));
}

PaintManager *MainScene::getPaintManager()
{
    return paintManager;
}

GraphicItemTerm *MainScene::getSelectedNode()
{
    return selectedNode;
}

void MainScene::showGroup(QString groupUuid)
{
    showGroup(QUuid(groupUuid));
}

void MainScene::showGroup(QUuid groupUuid)
{
    currGroupUuid = groupUuid;
    updateModel();
    updateSceneRect();
    sceneContentUpdated();
}

void MainScene::showAllGroups()
{
    locateGroupsVertically();
    resetPaintFlags();
    userInactiveTimer.start();
}

void MainScene::setAnimSpeed(int val)
{
    for (auto group : groupList) {
        group->setAnimSpeed(val);
    }
}

void MainScene::startAllGroupTimers()
{
    for (auto group : groupList) {
        group->startAnimation();
    }
}

void MainScene::stopAllGroupTimers()
{
    for (auto group : groupList)
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
    return getCurrNodeStringField([] (InfoTerm* node) { return node->getUuid().toString(); });
}

QString MainScene::getCurrNodeName()
{
    return getCurrNodeStringField([] (InfoTerm* node) { return node->getName(); });
}

QString MainScene::getCurrNodeForms()
{
    return getCurrNodeStringField([] (InfoTerm* node) { return node->getNameFormStr(); });
}

QString MainScene::getCurrNodeDefinition()
{
    return getCurrNodeStringField([] (InfoTerm* node) { return node->getDefinition(); });
}

QString MainScene::getCurrNodeDescription()
{
    return getCurrNodeStringField([] (InfoTerm* node) { return node->getDescription(); });
}

QString MainScene::getCurrNodeExamples()
{
    return getCurrNodeStringField([] (InfoTerm* node) { return node->getExamples(); });
}

QString MainScene::getCurrNodeGroupName()
{
    if (auto node = getSelectedNode()) {
        QUuid uuid = node->getGroupUuid();
        if (!uuid.isNull()) {
            return groupsMgr->getGroupName(uuid);
        }
    }
    return "";
}

bool MainScene::getCurrNodeIsRoot()
{
    if (auto node = getSelectedNode()) {
        return node->isRoot();
    }
    return false;
}

TermGroup *MainScene::getNearestNotPaintedGroup()
{
    TermGroup* nearestNotPaintedGroup = nullptr;

    for (auto group : groupList) {
        if (group->alreadyPainted) {
            continue;
        }

        if (nearestNotPaintedGroup == nullptr) {
            nearestNotPaintedGroup = group;
        } else {
            auto nearGroupCenter = nearestNotPaintedGroup->getGroupRect().center();
            auto currentGroupCenter = group->getGroupRect().center();
            auto sceneViewCenter = sceneViewRect.center();

            auto lineToNear = QLineF(nearGroupCenter, sceneViewCenter);
            auto lineToCurrent = QLineF(currentGroupCenter, sceneViewCenter);
            if (lineToCurrent.length() < lineToNear.length()) {
                nearestNotPaintedGroup = group;
            }
        }
    }

    return nearestNotPaintedGroup;
}

void MainScene::paintOneGroupIfNeed()
{
    if (paintManager->isPaintInProcessNow()) {
        userInactiveTimer.start();
        return;
    }

    if (auto paintGroup = getNearestNotPaintedGroup()) {

        paintGroup->alreadyPainted = true;

        paintManager->clearAllQueues();
        paintManager->addGroup(paintGroup);

//        userInactiveTimer.start();
    } else {
        qDebug() << "No paint";
    }
}

bool MainScene::hasSelection() {
    return getSelectedNode() != nullptr;
}

QRectF MainScene::getSceneRect() const
{
    return sceneRect;
}

GraphicItemTerm *MainScene::getNodeAtPoint(const QPointF &pt) const
{
    for (auto group : groupList) {
        if (group->getGroupRect().contains(pt)) {
            return group->getNodeAtPoint(pt);
        }
    }

    return nullptr;
}

void MainScene::findHover(const QPointF &atPt)
{
    if (mouseMoveReactionTimer.isActive()) {
        return;
    } else {
        mouseMoveReactionTimer.start();
    }

    bool needSignal = false;

    if (hoverNode != nullptr) {
        if (!hoverNode->getNodeRect(CoordType::scene).contains(atPt)) {
            hoverNode->setHover(false);
            paintManager->addNode(hoverNode, false);
            needSignal = true;
            hoverNode = nullptr;
        } else {
            return;
        }
    }

    if (auto node = getNodeAtPoint(atPt)) {
        node->setHover(true);
        hoverNode = node;
        paintManager->addNode(node, false);
        needSignal = true;
    }

    if (needSignal) {
        paintManager->sendPaintNodeSignal();
    }
}

void MainScene::findClick(const QPointF &atPt)
{
    if (selectedNode != nullptr) {
        if (!selectedNode->getNodeRect(CoordType::scene).contains(atPt)) {
            selectedNode->setSelection(false);
            paintManager->addNode(selectedNode, true);
            selectedNode = nullptr;
        } else {
            return;
        }
    }

    if (auto node = getNodeAtPoint(atPt)) {
        node->setSelection(true);
        selectedNode = node;
        paintManager->addNode(node, true);
    }

    if (selectedNode != nullptr) {
        someSelected();
    } else {
        selectionDrop();
    }
}

QString MainScene::getCurrNodeStringField(std::function<QString (InfoTerm*)> strFunction)
{
    if (auto node = getSelectedNode()) {
        return strFunction(node);
    }

    return "";
}

void MainScene::createTestGroups()
{
    groupsMgr->addNewGroup("TestGroup1", "");

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
