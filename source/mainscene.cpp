/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
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

#include "source/mainscene.h"

#include <QApplication>
#include <QThread>

#include "source/Database/database.h"
#include "source/Managers/notificationmanager.h"

MainScene::MainScene(GroupsManager* groupsMgr, QObject* parent)
    : QObject(parent)
    , mGroupBuilder(this)
{
    mouseMoveReactionTimer.setInterval(static_cast<int>(1000 / AppSettings::Scene::FPS));
    mouseMoveReactionTimer.setSingleShot(true);

    assert(groupsMgr != nullptr);
    this->groupsMgr = groupsMgr;
    connect(groupsMgr, &GroupsManager::groupAdded, this, &MainScene::checkGroupAddition);
    connect(groupsMgr, &GroupsManager::groupDeleted, this, &MainScene::checkGroupDeletion);
    connect(groupsMgr, &GroupsManager::nodeChanged, this, &MainScene::updateGroup);

    connect(&mGroupBuilder, &AsyncGroupBuilder::finished, this, &MainScene::takeBuildGroupAndShow, Qt::QueuedConnection);

    // Loading notification
    connect(&mGroupBuilder, &QThread::started, this, &MainScene::groupLoadingChanged);
    connect(&mGroupBuilder, &QThread::finished, this, &MainScene::groupLoadingChanged);
}

void MainScene::selectGroup(const QUuid groupUuid)
{
    assert(!groupUuid.isNull());
    setCurrentGroup(groupUuid);
}

void MainScene::selectTerm(const QUuid termUuid)
{
    if (auto* node = findTerm(termUuid))
        selectTerm(node);
}

void MainScene::dropGroup()
{
    dropTermSelection();

    mCurrentGroup.reset();

    setSceneRect(QRectF());

    updateEdgeCache();

    emit currentGroupChanged();
}

void MainScene::updateGroup()
{
    auto groupUuid = currentGroupUuid();
    selectGroup(groupUuid);
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

void MainScene::takeBuildGroupAndShow()
{
    if (auto group = mGroupBuilder.takeResult()) {
        // Can be nullptr if build thread was interrupted
        showNewGroup(group);
    }
}

void MainScene::showNewGroup(TermGroup* newGroup)
{
    assert(newGroup != nullptr);

    auto oldUuid = mCurrentGroup ? mCurrentGroup->uuid() : QUuid();
    auto newUuid = newGroup->uuid();

    bool differentGroups = oldUuid != newUuid;

    mCurrentGroup.reset(newGroup);

    assert(mCurrentGroup);

    mCurrentGroup->setBasePoint(QPointF(40, 40));

    updateSceneRect();

    if (differentGroups)
        emit currentGroupChanged();

    emit nodesChanged();
    updateEdgeCache();
}

void MainScene::setCurrentGroup(const QUuid& newGroupUuid)
{
    assert(!newGroupUuid.isNull());

    auto oldGroupUuid = currentGroupUuid();
    bool newGroup     = newGroupUuid != oldGroupUuid;

    dropTermSelection();

    // Taking groupUuid from parameter or current groupUuid
    QUuid tmpGroupUuid = newGroup ? newGroupUuid : oldGroupUuid;

    assert(!tmpGroupUuid.isNull());

    if (mGroupBuilder.isRunning()) {
        mGroupBuilder.requestInterruption();
        this->thread()->msleep(200);
    }

    if (!mGroupBuilder.isRunning()) {
        mGroupBuilder.setAction([this, groupUuid = tmpGroupUuid]() -> TermGroup* {
            auto* group = groupsMgr->createGroup(groupUuid);
            if (group->thread()->isInterruptionRequested())
                return nullptr;
            group->moveToThread(this->thread());
            return group;
        });

        mGroupBuilder.start();
    } else {
        qInfo("Bad luck");
    }
}

QQmlListProperty<PaintedTerm> MainScene::getNodes()
{
    return QQmlListProperty<PaintedTerm>(this, this, &MainScene::termCount, &MainScene::term);
}

void MainScene::updateSceneRect()
{
    if (!mCurrentGroup)
        return;

    auto baseRc = mCurrentGroup->getGroupRect();

    int       mV = 40;
    QMarginsF mrg(mV, mV, mV, mV);
    baseRc = baseRc.marginsAdded(mrg);
    setSceneRect(baseRc);
}

QStringList MainScene::search(const QString& text)
{
    QStringList ret;

    if (text.isEmpty())
        return ret;

    if (mCurrentGroup) {
        auto nearestUuids  = mCurrentGroup->searchNearest(text);
        auto containsUuids = mCurrentGroup->searchContains(text);

        for (auto uuid : nearestUuids)
            ret << uuid.toString();

        for (auto uuid : containsUuids) {
            auto insertVal = uuid.toString();
            if (!ret.contains(insertVal))
                ret << insertVal;
        }

        ret = ret.mid(0, 7);
    }

    return ret;
}

void MainScene::deleteSelectedTerm()
{
    if (auto* node = getSelectedTerm()) {
        dropTermSelection();
        groupsMgr->deleteNode(node->data().uuid);
    }
}

QPointF MainScene::getTermPosition(const QUuid termUuid) const
{
    if (auto* node = findTerm(termUuid))
        return node->getCenter(CoordType::scene);

    return QPointF();
}

void MainScene::setMouseClick(qreal x, qreal y) { findClick(QPointF(x, y)); }

PaintedTerm* MainScene::getSelectedTerm() const { return selectedTerm; }

PaintedTerm* MainScene::findTerm(const QUuid& termUuid) const
{
    return mCurrentGroup ? mCurrentGroup->getNode(termUuid) : nullptr;
}

void MainScene::selectTerm(PaintedTerm* term, bool needRepaint)
{
    if (selectedTerm != term) {
        // Drop selection
        if (selectedTerm != nullptr)
            selectedTerm->setSelection(false);

        // Set new selection
        selectedTerm = term;

        // Call selection
        if (selectedTerm != nullptr)
            selectedTerm->setSelection(true);

        emit selectionChanged();

        if (needRepaint) {
            updateEdgeCache();
        }
    }
}

void MainScene::dropTermSelection(bool needRepaint) { selectTerm(nullptr, needRepaint); }

QString MainScene::termUuidToName(const QUuid termUuid) const
{
    if (auto* node = findTerm(termUuid))
        return node->data().term;

    return "";
}

QUuid MainScene::termNameToUuid(const QString termName) const
{
    auto* node = mCurrentGroup ? mCurrentGroup->getNode(termName) : nullptr;
    return node ? node->data().uuid : QUuid();
}

TermDataWrapper MainScene::getCurrentNode()
{
    if (auto* node = getSelectedTerm()) {
        return TermDataWrapper(node->data());
    }

    return TermDataWrapper();
}

TermGroup* MainScene::getCurrentGroup() const { return mCurrentGroup.get(); }

bool MainScene::hasCurrentGroup() const { return !mCurrentGroup.isNull(); }

QString MainScene::getCurrNodeNameAndDefinition()
{
    if (auto* node = getSelectedTerm())
        return node->cache().termAndDefinition();

    return "";
}

QString MainScene::getCurrNodeHierarchyDefinition()
{
    if (auto* node = getSelectedTerm())
        return node->getHierarchyDefinition();

    return "";
}

QUuid MainScene::currentGroupUuid() const { return mCurrentGroup ? mCurrentGroup->uuid() : QUuid(); }

bool MainScene::isAnyNodeSelected() const { return getSelectedTerm() != nullptr; }

QRectF MainScene::sceneRect() const { return mSceneRect; }

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
        return mCurrentGroup->getNode(pt);

    return nullptr;
}

qsizetype MainScene::termCount() const { return mCurrentGroup ? mCurrentGroup->nodes().size() : 0; }

PaintedTerm* MainScene::term(qsizetype index) const
{
    if (!mCurrentGroup)
        return nullptr;

    if (index < 0 || index >= mCurrentGroup->nodes().size())
        return nullptr;

    return mCurrentGroup->nodes()[index];
}

qsizetype MainScene::termCount(QQmlListProperty<PaintedTerm>* list)
{
    return reinterpret_cast<MainScene*>(list->data)->termCount();
}

PaintedTerm* MainScene::term(QQmlListProperty<PaintedTerm>* list, qsizetype i)
{
    return reinterpret_cast<MainScene*>(list->data)->term(i);
}

QQmlListProperty<EdgeOld> MainScene::getEdges()
{
    return QQmlListProperty<EdgeOld>(this, this, &MainScene::edgeCount, &MainScene::edge);
}

qsizetype MainScene::edgeCount() const { return mCachedEdges.size(); }

EdgeOld* MainScene::edge(qsizetype index) const
{
    if (index < 0 || index >= mCachedEdges.size())
        return nullptr;

    return mCachedEdges[index];
}

qsizetype MainScene::edgeCount(QQmlListProperty<EdgeOld>* list)
{
    return reinterpret_cast<MainScene*>(list->data)->edgeCount();
}

EdgeOld* MainScene::edge(QQmlListProperty<EdgeOld>* list, qsizetype i)
{
    return reinterpret_cast<MainScene*>(list->data)->edge(i);
}

void MainScene::updateEdgeCache()
{
    mCachedEdges = mCurrentGroup ? mCurrentGroup->edgesForPaint() : EdgeOld::List();
    emit edgesChanged();
}

bool MainScene::isGroupLoading() const { return mGroupBuilder.isRunning(); }

void MainScene::findClick(const QPointF& atPt)
{
    // Check for click in same point
    if (auto selected = getSelectedTerm()) {
        // if same rect - exit
        if (selected->getNodeRect(CoordType::scene).contains(atPt)) {
            emit selectionDoubleClick();
            return;
        }
    }

    if (auto* node = getNodeAtPoint(atPt)) // Click in other node
        selectTerm(node);
    else
        dropTermSelection(true);
}
