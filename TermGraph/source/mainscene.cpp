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
#include "source/Managers/notificationmanager.h"

MainScene::MainScene(GroupsManager* groupsMgr, NodesManager* nodesMgr, PaintManager* paintManager, QObject* parent)
    : QObject(parent)
{
    mouseMoveReactionTimer.setInterval(static_cast<int>(1000/AppSettings::Scene::FPS));
    mouseMoveReactionTimer.setSingleShot(true);

    assert(groupsMgr != nullptr);
    this->groupsMgr = groupsMgr;
    connect(groupsMgr, &GroupsManager::groupAdded, this, &MainScene::checkGroupAddition);
    connect(groupsMgr, &GroupsManager::groupDeleted, this, &MainScene::checkGroupDeletion);

    assert(nodesMgr != nullptr);
    this->nodesMgr = nodesMgr;
    connect(nodesMgr, &NodesManager::nodeChanged, this, &MainScene::updateGroup);

    assert(paintManager != nullptr);
    this->paintManager = paintManager;
}

void MainScene::dropGroup()
{
    dropSelectedNode();

    paintManager->addClearRect(sceneRect(), true);
    paintManager->clearAllQueues();
    mCurrentGroup.reset();

    setSceneRect(QRectF());

    requestPaint(true);
}

void MainScene::updateGroup()
{
    auto groupUuid = currentGroupUuid();
    setCurrentGroup(groupUuid);
}

void MainScene::checkGroupAddition()
{
    // If group was added and we have no groups before, we must switch to it
    auto groupsUuids = groupsMgr->getAllUuidsSortedByLastEdit();
    if (groupsUuids.size() == 1)
        setCurrentGroup(groupsUuids.first());
}

void MainScene::checkGroupDeletion()
{
    // If group was deleted, and it was current group, we must delete it too
    auto currentGroup = currentGroupUuid();
    auto groupsUuids  = groupsMgr->getAllUuidsSortedByLastEdit();
    if (!groupsUuids.contains(currentGroup))
        dropGroup();
}

QQmlListProperty<PaintedTerm> MainScene::getNodes()
{
    return QQmlListProperty<PaintedTerm>(this, this, &MainScene::termCount, &MainScene::term);
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
    if (auto* node = getSelectedNode()) {
        dropSelectedNode();
        nodesMgr->deleteNode(node->getUuid());
    }
}

void MainScene::resetPaintFlags()
{
    if (!mCurrentGroup)
        return;

    mCurrentGroup->resetPaintFlags();
}

void MainScene::setMouseClick(qreal x, qreal y)
{
    findClick(QPointF(x,y));
}

PaintedTerm* MainScene::getSelectedNode() const
{
    return selectedNode;
}

void MainScene::dropSelectedNode(bool sendSignal)
{
    if (selectedNode != nullptr) {
        selectedNode->setSelection(false);
        selectedNode = nullptr;
        if (sendSignal)
            emit selectionChanged();
        checkGroupColors();
    }
}

void MainScene::checkGroupColors()
{
    if (mCurrentGroup)
        mCurrentGroup->checkColors();
}

void MainScene::setCurrentGroup(const QString& groupUuid)
{
    setCurrentGroup(QUuid(groupUuid));
}

void MainScene::setCurrentGroup(const QUuid& newGroupUuid)
{
    assert(!newGroupUuid.isNull());

    auto oldGroupUuid = QUuid(currentGroupUuid());
    bool newGroup     = newGroupUuid != oldGroupUuid;

    paintManager->addClearRect(sceneRect(), true);
    paintManager->clearAllQueues();

    dropSelectedNode();

    // Taking groupUuid from parameter or current groupUuid
    QUuid tmpGroupUuid = newGroup ? newGroupUuid : oldGroupUuid;

    assert(!tmpGroupUuid.isNull());

    auto* groupPtr = groupsMgr->createGroup(tmpGroupUuid);
    mCurrentGroup.reset(groupPtr);

    assert(mCurrentGroup);

    mCurrentGroup->sceneUpdateSignal();
    mCurrentGroup->setBasePoint(QPointF(40, 40));

    updateSceneRect();

    requestPaint(true);

    if (newGroup)
        emit currentGroupChanged();

    emit nodesChanged();
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
    if (auto* node = getSelectedNode()) {
        auto info = node->infoContainer();
        return NodeGadgetWrapper(info);
    }

    return NodeGadgetWrapper();
}

QString MainScene::getCurrNodeNameAndDefinition()
{
    if (auto* node = getSelectedNode())
        return node->getTermAndDefinition();

    return "";
}

QString MainScene::getCurrNodeHierarchyDefinition()
{
    if (auto* node = getSelectedNode())
        return node->getHierarchyDefinition();

    return "";
}

QString MainScene::currentGroupUuid() const
{
    return mCurrentGroup ? mCurrentGroup->getUuid().toString() : "";
}

QString MainScene::currentGroupName() const
{
    return mCurrentGroup ? mCurrentGroup->getName() : "";
}

bool MainScene::isAnyNodeSelected() const
{
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

int MainScene::termCount() const
{
    return mCurrentGroup ? mCurrentGroup->getAllNodes().size() : 0;
}

PaintedTerm *MainScene::term(int index) const
{
    if (!mCurrentGroup)
        return nullptr;

    if (index < 0 || index >= mCurrentGroup->getAllNodes().size())
        return nullptr;

    return mCurrentGroup->getAllNodes()[index];
}

int MainScene::termCount(QQmlListProperty<PaintedTerm> *list)
{
    return reinterpret_cast<MainScene*>(list->data)->termCount();
}

PaintedTerm *MainScene::term(QQmlListProperty<PaintedTerm> *list, int i)
{
    return reinterpret_cast<MainScene*>(list->data)->term(i);
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
            dropSelectedNode(false);
        }
    }

    if (auto node = getNodeAtPoint(atPt)) {  // Click in other node
        node->setSelection(true);
        selectedNode = node;
        checkGroupColors();
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
