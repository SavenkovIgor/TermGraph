/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#include "mainscene.h"

MainScene::MainScene(GroupsManager* groupsMgr, NodesManager* nodesMgr, PaintManager *paintManager)
{
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
}

void MainScene::initAllGroups()
{
    auto loadGroup = QUuid(currentGroupUuid());

    if (loadGroup.isNull()) {
        auto allGroupsUuids = groupsMgr->getAllUuidsSortedByLastEdit();
        if (!allGroupsUuids.isEmpty()) {
            loadGroup = allGroupsUuids.first();
        }
    }

    if (!loadGroup.isNull()) {
        auto* group = groupsMgr->createGroup(loadGroup);
        mCurrentGroup.reset(group);
    }
}

void MainScene::updateModel()
{
    paintManager->addClearRect(sceneRect(), true);
    paintManager->clearAllQueues();
    mCurrentGroup.reset();

    hoverNode = nullptr;
    selectedNode = nullptr;

    initAllGroups();

    if (mCurrentGroup)
        mCurrentGroup->sceneUpdateSignal();

    locateGroupsVertically();
    updateSceneRect();

    requestPaint(true);
    qDebug() << "model updated";
}

void MainScene::locateGroupsVertically()
{
    if (!mCurrentGroup)
        return;

    // Выставляем позиции групп
    auto  basePt = QPointF(40, 40);
    mCurrentGroup->setBasePoint(basePt);
}

void MainScene::updateSceneRect()
{
    if (!mCurrentGroup)
        return;

    auto baseRc = mCurrentGroup->baseRect->getRect(CoordType::scene);

    int       mV = 40;
    QMarginsF mrg(mV, mV, mV, mV);
    baseRc = baseRc.marginsAdded(mrg);
    setSceneRect(baseRc);
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

QColor MainScene::getSceneBackgroundColor() const
{
    return AppStyle::Colors::Scene::background;
}

void MainScene::resetPaintFlags()
{
    if (!mCurrentGroup)
        return;

    mCurrentGroup->resetPaintFlags();
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

void MainScene::dropSelection()
{
    if (selectedNode != nullptr) {
        selectedNode->setSelection(false);
        selectedNode = nullptr;
        emit selectionChanged();
    }
}

void MainScene::dropHover()
{
    hoverNode = nullptr;
}

void MainScene::setCurrentGroup(const QString& groupUuid)
{
    setCurrentGroup(QUuid(groupUuid));
}

void MainScene::setCurrentGroup(const QUuid& groupUuid)
{
    dropSelection();
    dropHover();

    if (mCurrGroupUuid != groupUuid) {
        mCurrGroupUuid = groupUuid;
        emit currentGroupChanged();
    }

    updateModel();
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
    if (auto node = getSelectedNode())
        return node->getHierarchyDefinition();

    return "";
}

QString MainScene::currentGroupUuid()
{
    return mCurrGroupUuid.toString();
}

bool MainScene::isAnyNodeSelected() {
    return getSelectedNode() != nullptr;
}

QRectF MainScene::sceneRect() const
{
    return mSceneRect;
}

void MainScene::setSceneRect(const QRectF& newRect)
{
    if (mSceneRect != newRect) {
        mSceneRect = newRect;
        emit sceneRectChanged();
    }
}

PaintedTerm* MainScene::getNodeAtPoint(const QPointF& pt) const
{
    if (!mCurrentGroup)
        return nullptr;

    if (mCurrentGroup->getGroupRect().contains(pt))
        return mCurrentGroup->getNodeAtPoint(pt);

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
    if (paintAll)
        paintManager->addClearRect(sceneRect(), true);

    if (mCurrentGroup)
        paintManager->addGroup(mCurrentGroup.get(), paintAll, false);

    //    paintManager->addRect(sceneRect);

    if (requestPaint)
        paintManager->sendPaintGroupSignal();
}
