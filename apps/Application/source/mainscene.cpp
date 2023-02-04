// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/mainscene.h"

#include <QThread>

MainScene::MainScene(GroupsManager* groupsMgr, QObject* parent)
    : QObject(parent)
    , mGroupBuilder(this)
    , mTermsModel(new TermsModel(this))
    , mEdgesModel(new EdgesModel(this))
{
    assert(groupsMgr != nullptr);
    this->groupsMgr = groupsMgr;
    connect(groupsMgr, &GroupsManager::groupLoaded, this, &MainScene::createLoadedGroup);
    connect(groupsMgr, &GroupsManager::groupAdded, this, &MainScene::checkGroupAddition);
    connect(groupsMgr, &GroupsManager::groupDeleted, this, &MainScene::checkGroupDeletion);
    connect(groupsMgr, &GroupsManager::termChanged, this, &MainScene::updateGroup);

    connect(&mGroupBuilder, &AsyncGroupBuilder::finished, this, &MainScene::takeBuildGroupAndShow, Qt::QueuedConnection);

    // Loading notification
    connect(&mGroupBuilder, &QThread::started, this, &MainScene::groupLoadingChanged);
    connect(&mGroupBuilder, &QThread::finished, this, &MainScene::groupLoadingChanged);

    connect(this, &MainScene::selectionChanged, mTermsModel, &TermsModel::updateSelection);
}

void MainScene::selectGroup(const QUuid groupUuid)
{
    auto uuid = GroupUuid::from(groupUuid);
    assert(uuid.has_value());
    setCurrentGroup(*uuid);
}

void MainScene::selectTerm(const QUuid termUuid)
{
    if (auto node = findTerm(termUuid); node.has_value()) {
        selectTerm(node.value());
    }
}

void MainScene::dropGroup()
{
    dropTermSelection();

    mCurrentGroup = std::nullopt;

    setSceneRect(QRectF());

    mTermsModel->clear();
    mEdgesModel->clear();

    emit edgesChanged();
    emit currentGroupChanged();
}

void MainScene::updateGroup()
{
    if (auto uuid = currentGroupUuid()) {
        selectGroup(*uuid);
    }
}

void MainScene::checkGroupAddition()
{
    // If group was added and we have no groups before, we must switch to it
    auto groupsUuids = groupsMgr->getAllUuidsSortedByLastEdit();
    if (groupsUuids.size() == 1) {
        setCurrentGroup(groupsUuids.front());
    }
}

void MainScene::checkGroupDeletion()
{
    using namespace std;

    // If group was deleted, and it was current group, we must delete it too
    auto currentGroup = currentGroupUuid();
    if (!currentGroup) {
        return;
    }

    auto groupsUuids  = groupsMgr->getAllUuidsSortedByLastEdit();
    if (find(begin(groupsUuids), end(groupsUuids), *currentGroup) == groupsUuids.end()) {
        dropGroup();
    }
}

void MainScene::takeBuildGroupAndShow()
{
    if (auto group = mGroupBuilder.takeResult()) {
        // Can be nullptr if build thread was interrupted
        showNewGroup(group);
    }
}

void MainScene::createLoadedGroup()
{
    if (mGroupBuilder.isRunning()) {
        mGroupBuilder.requestInterruption();
        this->thread()->msleep(200);
    }

    if (!mGroupBuilder.isRunning()) {
        mGroupBuilder.setAction([this]() -> TermGroup::OptPtr {

            auto group = groupsMgr->createGroup();

            if (!group.has_value()) {
                return std::nullopt;
            }

            if (group.value()->thread()->isInterruptionRequested()) {
                return std::nullopt;
            }

            group.value()->moveToThread(this->thread());
            return group;
        });

        mGroupBuilder.start();
    } else {
        qInfo("Bad luck");
    }
}

void MainScene::showNewGroup(TermGroup::OptPtr newGroup)
{
    assert(newGroup.has_value());

    auto oldUuid = mCurrentGroup ? mCurrentGroup.value()->uuid() : QUuid();
    auto newUuid = newGroup.value()->uuid();

    bool differentGroups = oldUuid != newUuid;

    mCurrentGroup = newGroup;

    assert(mCurrentGroup.has_value());

    mCurrentGroup.value()->setBasePoint(QPointF(40, 40));

    updateSceneRect();

    if (mCurrentGroup) {
        mTermsModel->setGroup(mCurrentGroup);
        mEdgesModel->setGroup(mCurrentGroup);
    }

    if (differentGroups) {
        emit currentGroupChanged();
    }

    emit nodesChanged();
    emit edgesChanged();
}

void MainScene::setCurrentGroup(const GroupUuid& newGroupUuid)
{
    auto oldGroupUuid = currentGroupUuid();
    bool newGroup     = newGroupUuid != oldGroupUuid;

    dropTermSelection();

    // Taking groupUuid from parameter or current groupUuid
    Opt<GroupUuid> tmpGroupUuid = newGroup ? newGroupUuid : oldGroupUuid;

    if (!tmpGroupUuid) {
        return;
    }

    groupsMgr->loadGroup(*tmpGroupUuid);
}

void MainScene::updateSceneRect()
{
    if (!mCurrentGroup) {
        return;
    }

    auto baseRc = mCurrentGroup.value()->getGroupRect();

    int       mV = 40;
    QMarginsF mrg(mV, mV, mV, mV);
    baseRc = baseRc.marginsAdded(mrg);
    setSceneRect(baseRc);
}

QStringList MainScene::search(const QString& text)
{
    QStringList ret;

    if (text.isEmpty()) {
        return ret;
    }

    if (mCurrentGroup) {
        auto nearestUuids  = mCurrentGroup.value()->searchNearest(text);
        auto containsUuids = mCurrentGroup.value()->searchContains(text);

        for (auto uuid : nearestUuids) {
            ret << uuid.toString();
        }

        for (auto uuid : containsUuids) {
            auto insertVal = uuid.toString();
            if (!ret.contains(insertVal)) {
                ret << insertVal;
            }
        }

        ret = ret.mid(0, 7);
    }

    return ret;
}

void MainScene::deleteSelectedTerm()
{
    if (auto node = getSelectedTerm(); node.has_value()) {
        dropTermSelection();
        if (auto uuid = node.value()->data().uuid) {
            groupsMgr->deleteNode(*uuid);
        }
    }
}

QPointF MainScene::getTermPosition(const QUuid termUuid) const
{
    if (auto node = findTerm(termUuid); node.has_value()) {
        return node.value()->getCenter(CoordType::scene);
    }

    return {};
}

void MainScene::setMouseClick(qreal x, qreal y) { findClick(QPointF(x, y)); }

PaintedTerm::OptPtr MainScene::getSelectedTerm() const { return selectedTerm; }

PaintedTerm::OptPtr MainScene::findTerm(const QUuid& termUuid) const
{
    return mCurrentGroup ? mCurrentGroup.value()->getTerm(termUuid) : std::nullopt;
}

void MainScene::selectTerm(PaintedTerm::OptPtr term)
{
    if (!mCurrentGroup) {
        return;
    }

    if (selectedTerm != term) {
        // Drop selection
        if (selectedTerm.has_value()) {
            mCurrentGroup.value()->selectTerm(selectedTerm.value(), false);
        }

        // Set new selection
        selectedTerm = term;

        // Call selection
        if (selectedTerm.has_value()) {
            mCurrentGroup.value()->selectTerm(selectedTerm.value(), true);
        }

        emit selectionChanged();
        emit edgesChanged();
    }
}

void MainScene::dropTermSelection() { selectTerm(std::nullopt); }

QString MainScene::termUuidToName(const QUuid termUuid) const
{
    if (auto node = findTerm(termUuid); node.has_value()) {
        return node.value()->data().term;
    }

    return "";
}

QUuid MainScene::termNameToUuid(const QString termName) const
{
    auto node = mCurrentGroup ? mCurrentGroup.value()->getTerm(termName) : std::nullopt;
    if (!node) {
        return {};
    }

    if (!((*node)->data().uuid)) {
        return {};
    }

    return (*node)->data().uuid->get();
}

TermDataWrapper MainScene::getCurrentNode()
{
    if (auto node = getSelectedTerm(); node.has_value()) {
        return TermDataWrapper(node.value()->data());
    }

    return {};
}

TermGroup* MainScene::getCurrentGroup() const { return mCurrentGroup.value().get(); }

bool MainScene::hasCurrentGroup() const { return mCurrentGroup.has_value(); }

QString MainScene::getCurrNodeNameAndDefinition()
{
    if (auto node = getSelectedTerm(); node.has_value()) {
        return node.value()->cache().termAndDefinition();
    }

    return "";
}

QString MainScene::getCurrNodeHierarchyDefinition()
{
    if (!mCurrentGroup) {
        return "";
    }

    if (auto node = getSelectedTerm(); node.has_value()) {
        return mCurrentGroup.value()->getHierarchyDefinition(node.value());
    }

    return "";
}

Opt<GroupUuid> MainScene::currentGroupUuid() const {
    return mCurrentGroup ? GroupUuid::from(mCurrentGroup.value()->uuid()) : std::nullopt;
}

bool MainScene::isAnyNodeSelected() const { return getSelectedTerm().has_value(); }

QRectF MainScene::sceneRect() const { return mSceneRect; }

void MainScene::setSceneRect(const QRectF& newRect)
{
    if (mSceneRect != newRect) {
        mSceneRect = newRect;
        emit sceneRectChanged();
    }
}

PaintedTerm::OptPtr MainScene::getNodeAtPoint(const QPointF& pt) const
{
    if (!mCurrentGroup) {
        return std::nullopt;
    }

    if (mCurrentGroup.value()->getGroupRect().contains(pt)) {
        return mCurrentGroup.value()->getTerm(pt);
    }

    return std::nullopt;
}

bool MainScene::isGroupLoading() const { return mGroupBuilder.isRunning(); }

void MainScene::findClick(const QPointF& atPt)
{
    // Check for click in same point
    if (auto selected = getSelectedTerm(); selected.has_value()) {
        // if same rect - exit
        if (selected.value()->getNodeRect(CoordType::scene).contains(atPt)) {
            emit selectionDoubleClick();
            return;
        }
    }

    if (auto node = getNodeAtPoint(atPt); node.has_value()) { // Click in other node
        selectTerm(node);
    } else {
        dropTermSelection();
    }
}