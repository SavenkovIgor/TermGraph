#include "mainscene.h"

MainScene::MainScene(GroupsManager* groupsMgr, NodesManager* nodesMgr, PaintManager *paintManager)
{
    sceneRhytm.setSingleShot(false);
    sceneRhytm.setInterval(30);

    mouseMoveReactionTimer.setInterval(static_cast<int>(1000/AppSettings::Scene::FPS));
    mouseMoveReactionTimer.setSingleShot(true);

    assert(groupsMgr != nullptr);
    this->groupsMgr = groupsMgr;
    connect(groupsMgr, &GroupsManager::groupsListChanged, this, &MainScene::updateModel);

    assert(nodesMgr != nullptr);
    this->nodesMgr = nodesMgr;
    connect(nodesMgr, &NodesManager::nodeChanged, this, &MainScene::updateModel);

    assert(paintManager != nullptr);
    this->paintManager = paintManager;
}

MainScene::~MainScene()
{
    deleteAllGroups();
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
    connect(&sceneRhytm, &QTimer::timeout, group, &TermGroup::sceneUpdateSignal);
    groupList << group;
}

void MainScene::deleteAllGroups()
{
    for (auto group : groupList) {
        delete group;
    }

    groupList.clear();
}

void MainScene::updateModel()
{
    sceneRhytm.stop();
    stopAllGroupTimers();

    paintManager->addClearRect(sceneRect, true);
    paintManager->clearAllQueues();
    deleteAllGroups();

    hoverNode = nullptr;
    selectedNode = nullptr;

    initAllGroups();

    for (auto group : groupList) {
        group->sceneUpdateSignal();
    }

    locateGroupsVertically();

    sceneRhytm.start();
    // startAllGroupTimers();
    updateSceneRect();
    emit sceneContentUpdated();

    requestPaint(true);
    qDebug() << "model updated";
}

void MainScene::locateGroupsVertically()
{
    qreal y = 40;
    qreal x = 40;

    // Выставляем позиции групп
    for (TermGroup* group : groupList) {
        auto baseRc = group->baseRect->getRect(CoordType::scene);
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

        auto baseRc = group->baseRect->getRect(CoordType::scene);
        allRect = allRect.united(baseRc);
    }

    int mV = 40;
    QMarginsF mrg(mV, mV, mV, mV);
    auto withMargins = allRect.marginsAdded(mrg);
    sceneRect = withMargins;
}

void MainScene::centerViewOn(QPointF point)
{

}

void MainScene::deleteSelectedNode()
{
    if (auto node = getSelectedNode()) {
        dropSelection();
        nodesMgr->deleteNode(node->getUuid());
    }
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
//        emit sceneContentUpdated();
    }
}

QColor MainScene::getSceneBackgroundColor() const
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

PaintedTerm *MainScene::getSelectedNode()
{
    return selectedNode;
}

void MainScene::dropSelection(bool withSignal)
{
    if (selectedNode != nullptr) {
        selectedNode->setSelection(false);
        selectedNode = nullptr;
    }

    if (withSignal) {
        emit selectionChanged();
    }
}

void MainScene::dropHover()
{
    hoverNode = nullptr;
}

void MainScene::showGroup(const QString &groupUuid)
{
    showGroup(QUuid(groupUuid));
}

void MainScene::showGroup(const QUuid &groupUuid)
{
    QElapsedTimer groupChangeTimer;
    groupChangeTimer.start();

    dropSelection(false);
    dropHover();

    currGroupUuid = groupUuid;
    updateModel();
    updateSceneRect();
    emit sceneContentUpdated();

    qDebug() << "Group changed in: " << groupChangeTimer.restart();
}

void MainScene::showAllGroups()
{
    locateGroupsVertically();
    resetPaintFlags();
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

NodeGadgetWrapper MainScene::getCurrentNode()
{
    if (auto node = getSelectedNode()) {
        auto info = node->infoContainer();
        return NodeGadgetWrapper(info);
    }

    return NodeGadgetWrapper();
}

QString MainScene::getCurrNodeNameAndDefinition()
{
    if (auto node = getSelectedNode())
        return node->getTermAndDefinition();

    return "";
}

QString MainScene::getCurrNodeHierarchyDefinition()
{
    if (auto node = getSelectedNode()) {
        return node->getHierarchyDefinition();
    }

    return "";
}

QString MainScene::getCurrGroupUuid()
{
    if (groupList.isEmpty()) {
        return "";
    }

    return groupList.first()->getUuid().toString();
}

TermGroup *MainScene::getNearestNotPaintedGroup()
{
    TermGroup* nearestNotPaintedGroup = nullptr;

    for (auto group : groupList) {
        if (!group->needPaint()) {
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

bool MainScene::isAnyNodeSelected() {
    return getSelectedNode() != nullptr;
}

QRectF MainScene::getSceneRect() const
{
    return sceneRect;
}

PaintedTerm *MainScene::getNodeAtPoint(const QPointF &pt) const
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
    // Check for click in same point
    if (auto selected = getSelectedNode()) {
        // if same rect - exit
        if (selected->getNodeRect(CoordType::scene).contains(atPt)) {
            emit selectionDoubleClick();
            return;
        } else {

            // Else drop current selection
            selectedNode->setSelection(false);
            selectedNode = nullptr;
        }
    }

    if (auto node = getNodeAtPoint(atPt)) {  // Click in other node
        node->setSelection(true);
        selectedNode = node;
    }

    requestPaint(true);
    emit selectionChanged();
}

void MainScene::createTestGroups()
{
    QString groupName = "TestGroup1";
    groupsMgr->addNewGroup(groupName, "");

    auto groupUuid = groupsMgr->getGroupUuid(groupName);

    nodesMgr->addNewNode("1", "", "", "", groupUuid.toString());
    nodesMgr->addNewNode("2", "", "", "", groupUuid.toString());
    nodesMgr->addNewNode("3", "{1}{2}", "", "", groupUuid.toString());
    nodesMgr->addNewNode("4", "{1}{2}", "", "", groupUuid.toString());
    nodesMgr->addNewNode("5", "{1}", "", "", groupUuid.toString());
    nodesMgr->addNewNode("6", "{5}", "", "", groupUuid.toString());
    nodesMgr->addNewNode("7", "{5}", "", "", groupUuid.toString());
    nodesMgr->addNewNode("8", "{6}{7}", "", "", groupUuid.toString());
    nodesMgr->addNewNode("9", "", "", "", groupUuid.toString());
}

void MainScene::requestPaint(bool paintAll)
{
    sendGroupsToPaintManager(true, paintAll);
}

void MainScene::sendGroupsToPaintManager(bool requestPaint, bool paintAll)
{
    if (paintAll) {
        paintManager->addClearRect(sceneRect, true);
    }

    for (auto group : groupList) {
        paintManager->addGroup(group, paintAll, false);
    }

//    paintManager->addRect(sceneRect);

    if (requestPaint) {
        paintManager->sendPaintGroupSignal();
    }
}
