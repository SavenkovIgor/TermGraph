#include "mainscene.h"

MainScene::MainScene(GroupsManager* groupsMgr, NodesManager* nodesMgr) : QGraphicsScene()
{
    sceneRhytm.setSingleShot(false);
    sceneRhytm.setInterval(30);

    selectTimer.setSingleShot(false);
    selectTimer.setInterval(200);
    connect(&selectTimer, SIGNAL(timeout()), SLOT(checkSelection()));

    userInactiveTimer.setInterval(static_cast<int>(1000/AppConfig::SceneSettings::FPS));
    userInactiveTimer.setSingleShot(true);
    connect(&userInactiveTimer, SIGNAL(timeout()), SLOT(paintOneGroupIfNeed()));

    mouseMoveReactionTimer.setInterval(static_cast<int>(1000/AppConfig::SceneSettings::FPS));
    mouseMoveReactionTimer.setSingleShot(true);

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

GraphicItemTerm::List MainScene::getAllNodes()
{
    GraphicItemTerm::List ret;
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
    GraphicItemTerm::List nodesList = getAllTermsAtPoint(evt->scenePos());
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
        for (GraphicItemTerm* node : getAllNodes()) {
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
    userInactiveTimer.stop();
    stopAllGroupTimers();

    paintManager->sendCleanAllSignal();
    deleteAllGroups();
    initAllGroups();

    for (TermGroup* group : groupList) {
        group->sceneUpdateSignal();
    }

    locateGroupsVertically();

    selectTimer.start();
//    viewGrpTimer.start(200);
    sceneRhytm.start();
    // startAllGroupTimers();
    sceneUpdated();
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

    for (TermGroup* group : groupList) {
        if (!group->baseRect->isVisible()) {
            continue;
        }

        QRectF baseRc = group->baseRect->rect().translated(group->baseRect->scenePos());
        allRect = allRect.united(baseRc);
    }

    int mV = 40;
    QMarginsF mrg(mV, mV, mV, mV);
    auto withMargins = allRect.marginsAdded(mrg);
    sceneRect = withMargins;
    setSceneRect(sceneRect);
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
    if (GraphicItemTerm* node = getSelected()) {
        nodesMgr->deleteNode(node->getUuid());
    }
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

void MainScene::startCheckTimer()
{
    elTimer.start();
}

void MainScene::restartCheckTimer(QString label)
{
    qDebug() << label << " " << elTimer.restart();;
}

void MainScene::setSceneViewRect(int x, int y, int width, int height)
{
    sceneViewRect = QRectF(x, y, width, height);

    auto tmpRect = sceneRect;
    tmpRect.setWidth(qMax(sceneRect.width(), sceneViewRect.width()));
    tmpRect.setHeight(qMax(sceneRect.height(), sceneViewRect.height()));

    if (tmpRect != sceneRect) {
        sceneRect = tmpRect;
        resetPaintFlags();
        sceneUpdated();
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
    mousePos = QPointF(x,y);
    findHover();
}

void MainScene::setMouseClick(qreal x, qreal y)
{
    mousePos = QPointF(x,y);
    findClick();
}

PaintManager *MainScene::getPaintManager()
{
    return paintManager;
}

void MainScene::showGroup(int num)
{
    QStringList allGroupNames = groupsMgr->getAllGroupsNames();

    if (num < 0 || num >= allGroupNames.size()) {
        return;
    }

    showGroup(allGroupNames[num]);
}

GraphicItemTerm *MainScene::getSelected()
{
    return selectedNode;
}

GraphicItemTerm::List MainScene::getAllTermsAtPoint(QPointF point) {
    GraphicItemTerm::List ret;
    for (GraphicItemTerm* node : getAllNodes()) {
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
    for (GraphicItemTerm* node : getAllNodes()) {
        if (node->isSelected()) {
//            someSelected();
            someSel = true;
            PaintedTerm::someoneSelect = true;
            node->setRelatedPaint(true);
        }
    }

    if (!someSel) {
//        selectionDrop();
        PaintedTerm::someoneSelect = false;
        if (!PaintedTerm::someoneHover) {
            for (GraphicItemTerm* node : getAllNodes()) {
                node->setRelatedPaint(false);
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
    if (GraphicItemTerm* node = getSelected()) {
        QUuid uuid = node->getGroupUuid();
        if (!uuid.isNull()) {
            return groupsMgr->getGroupName(uuid);
        }
    }
    return "";
}

bool MainScene::getCurrNodeIsRoot()
{
    if (GraphicItemTerm* node = getSelected()) {
        return node->isRoot();
    }
    return false;
}

TermGroup *MainScene::getNearestNotPaintedGroup()
{
    TermGroup* nearestNotPaintedGroup = nullptr;

    for (auto group: groupList) {
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

        paintManager->clearGroupsQueue();
        paintManager->addGroup(paintGroup);

        userInactiveTimer.start();
    } else {
        qDebug() << "No paint";
    }
}

bool MainScene::hasSelection() {
    return getSelected() != nullptr;
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

void MainScene::findHover()
{
    if (mouseMoveReactionTimer.isActive()) {
        return;
    } else {
        mouseMoveReactionTimer.start();
    }

    if (hoverNode != nullptr) {
        if (!hoverNode->getNodeRect(CoordType::scene).contains(mousePos)) {
            hoverNode->setHover(false);
            paintManager->addNode(hoverNode, true);
            hoverNode = nullptr;
        } else {
            return;
        }
    }

    if (auto node = getNodeAtPoint(mousePos)) {
        node->setHover(true);
        hoverNode = node;
        paintManager->addNode(node, true);
    }
}

void MainScene::findClick()
{
    if (selectedNode != nullptr) {
        if (!selectedNode->getNodeRect(CoordType::scene).contains(mousePos)) {
            selectedNode->setSelection(false);
            paintManager->addNode(selectedNode, true);
            selectedNode = nullptr;
        } else {
            return;
        }
    }

    if (auto node = getNodeAtPoint(mousePos)) {
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
    if (InfoTerm* node = getSelected()) {
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
