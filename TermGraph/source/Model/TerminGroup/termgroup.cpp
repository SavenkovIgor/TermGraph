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

#include "termgroup.h"

#include "source/Helpers/appstyle.h"
#include "source/Helpers/fonts.h"
#include "source/Helpers/tagprocessor.h"

int TermGroup::animSpeed = 300;

TermGroup::TermGroup(const GroupInfoContainer& info, QObject* parent)
    : QObject(parent)
    , TermGroupInfo(info)
{}

TermGroup::~TermGroup()
{
    //    qDebug() << "Delete group" << getName();
    checkSwapTimer.stop();
    animTimer.stop();

    delete orphansRect;
    delete baseRect;
}

void TermGroup::initNewNodes()
{
    loadEdges();
    removeCycles();
    removeExceedEdges();

    baseRect = new RectGraphicItem(QPoint(0, 0), QSizeF(10.0, 10.0));

    orphansRect = new RectGraphicItem();
    orphansRect->setParentItem(baseRect);

    setLevels();
    initTrees();
    addTreeRectsToScene();

    addOrphansToParents();
    addEdgesToParents();

    setTreeCoords();
    setNeighbours();
    setOrphCoords();

    checkSwapTimer.setSingleShot(false);
    setAnimSpeed(300);
    connect(&checkSwapTimer, &QTimer::timeout, this, &TermGroup::checkSwap);

    animTimer.setSingleShot(false);
    animTimer.setInterval(50);
    connect(&animTimer, &QTimer::timeout, this, &TermGroup::animateGroup);

    checkColors(true);
}

void TermGroup::loadNodes(const PaintedTerm::List& newNodes)
{
    clearNodesList();

    for (auto node : newNodes) {
        Q_ASSERT_X(node->getGroupUuid() == this->getUuid(), Q_FUNC_INFO, "Node group error");

        if (node->getGroupUuid() != this->getUuid())
            continue;

        addNodeToList(node);
    }

    initNewNodes();
}

PaintedTerm* TermGroup::getNode(const QPointF& pt) const
{
    for (auto tree : trees) {
        if (tree->getTreeRect(CoordType::scene).contains(pt)) {
            return tree->getNodeAtPoint(pt);
        }
    }

    auto orphansRect = QRectF(this->orphansRect->scenePos(), this->orphansRect->getSize());
    if (orphansRect.contains(pt)) {
        for (auto orphan : getOrphanNodes()) {
            if (orphan->getNodeRect(CoordType::scene).contains(pt)) {
                return orphan;
            }
        }
    }

    return nullptr;
}

PaintedTerm* TermGroup::getNode(const QUuid nodeUuid) const
{
    for (auto* node : getAllNodes())
        if (node->getUuid() == nodeUuid)
            return node;

    return nullptr;
}

void TermGroup::addOrphansToParents()
{
    for (auto node : getOrphanNodes()) {
        node->setParentItem(orphansRect);
    }
}

void TermGroup::addEdgesToParents()
{
    for (Edge* edge : getAllEdges()) {
        for (auto tree : trees) {
            if (tree->hasEdge(edge)) {
                edge->setParentItem(tree->rect);
            }
        }
    }
}

void TermGroup::checkColors(bool init)
{
    if (init) {
        for (auto* node : getAllNodes())
            node->initColor();

        return;
    }

    for (auto* node : getAllNodes())
        node->checkColor();
}

void TermGroup::startAnimation()
{
    animTimer.start();
    checkSwapTimer.start();
}

void TermGroup::stopAnimation()
{
    animTimer.stop();
    checkSwapTimer.stop();
}

void TermGroup::sceneUpdateSignal()
{
    updateGroupFrame();
}

void TermGroup::checkSwap()
{
    for (auto tree : trees)
        tree->checkSwap();
}

void TermGroup::animateGroup()
{
    for (auto tree : trees)
        tree->animateTree();
}

void TermGroup::addTreeRectsToScene()
{
    for (auto tree : trees)
        tree->rect->setParentItem(baseRect);
}

QSizeF TermGroup::getNameSize() const
{
    return Fonts::getTextMetrics(getName(), Fonts::getWeightFont());
}

qreal TermGroup::getGroupMinWidth()
{
    qreal width = 0.0;

    qreal groupNameWidth = getNameSize().width();
    qreal treeWidth      = getAllTreesSize().width();
    qreal orphansWidth   = NodeVerticalStackTools::getNodeVerticalStackedSize(getOrphanNodes()).width();

    width = std::max(width, groupNameWidth);
    width = std::max(width, treeWidth);
    width = std::max(width, orphansWidth);

    return width;
}

void TermGroup::setAnimSpeed(int val)
{
    checkSwapTimer.setInterval(static_cast<int>(val * 1.5));
    for (auto tree : trees) {
        tree->swAnim1.setDuration(val);
        tree->swAnim2.setDuration(val);
    }
    animSpeed = val;
}

int TermGroup::getAnimSpeed()
{
    return animSpeed;
}

UuidList TermGroup::searchNearest(const QString& text, int limit)
{
    QString                  searchText = text.toLower();
    QList<QPair<int, QUuid>> searchResults;
    // Taking distances
    for (auto* node : getAllNodes()) {
        auto lowerTerm = node->getCachedLowerTerm();

        // Exact match
        if (searchText == lowerTerm) {
            searchResults << QPair(0, node->getUuid());
            continue;
        }

        auto cuttedTerm = lowerTerm.left(searchText.size());  // Compare only left n characters

        auto acceptableLimit = static_cast<int>(cuttedTerm.size() * 0.25);
        auto distance        = TagProcessor::getLevDistance(cuttedTerm, searchText, acceptableLimit);

        if (distance <= acceptableLimit)
            searchResults << QPair(distance, node->getUuid());
    }

    // Sorting
    auto order = [](const QPair<int, QUuid>& s1, const QPair<int, QUuid>& s2) { return s1.first < s2.first; };
    std::sort(searchResults.begin(), searchResults.end(), order);

    // Removing numbers
    UuidList ret;
    int      count = 0;
    for (auto [dist, uuid] : searchResults) {
        if (count >= limit)
            break;

        ret.push_back(uuid);

        count++;
    }

    return ret;
}

UuidList TermGroup::searchContains(const QString& text, int limit)
{
    UuidList ret;
    auto     lowerSearch = text.toLower();

    for (auto* node : getAllNodes()) {
        if (node->getCachedLowerTerm().contains(lowerSearch))
            ret.push_back(node->getUuid());

        if (static_cast<int>(ret.size()) == limit)
            break;
    }

    return ret;
}

void TermGroup::setBasePoint(QPointF pt)
{
    baseRect->setPos(pt);
}

void TermGroup::updateRectsPositions()
{
    qreal   vSpacer = AppStyle::Sizes::groupVerticalSpacer;
    qreal   hSpacer = AppStyle::Sizes::groupHorizontalSpacer;
    QPointF basePoint(QPointF(hSpacer, vSpacer));

    auto nameSize = getNameSize();

    // Устанавливаем базовую точку имени
    basePoint.ry() += nameSize.height() + vSpacer;

    // Вычисляем под дерево
    for (auto tree : trees) {
        auto treeSize = tree->getTreeSize();
        tree->rect->setPos(basePoint);
        tree->rect->setSize(treeSize);
        basePoint.ry() += treeSize.height() + vSpacer;
    }

    QSizeF orphansSize = getOrphansSize();
    // Вычисляем под несвязанные вершины
    orphansRect->setPos(basePoint);
    orphansRect->setSize(orphansSize);  // Применяем
}

void TermGroup::updateBaseRectSize()
{
    QSizeF nameSize    = getNameSize();
    QSizeF treesSize   = getAllTreesSize();
    QSizeF orphansSize = getOrphansSize();
    qreal  vSpacer     = AppStyle::Sizes::groupVerticalSpacer;
    qreal  hSpacer     = AppStyle::Sizes::groupHorizontalSpacer;

    qreal width  = 0.0;
    qreal height = 0.0;

    width = std::max(width, nameSize.width());
    width = std::max(width, treesSize.width());
    width = std::max(width, orphansSize.width());
    width += hSpacer * 2;

    height += vSpacer;
    height += nameSize.height();
    if (treesSize.height() > 0)
        height += treesSize.height() + vSpacer;
    if (orphansSize.height() > 0)
        height += orphansSize.height() + vSpacer;
    height += vSpacer;

    baseRect->setSize(QSizeF(width, height));
}

void TermGroup::updateGroupFrame()
{
    updateRectsPositions();
    updateBaseRectSize();
}

void TermGroup::setOrphCoords(qreal maxWidth)
{
    auto orphansList = getOrphanNodes();
    if (orphansList.isEmpty()) {
        return;
    }

    for (int i = 0; i < orphansList.size(); i++) {
        int nMax = i;
        for (int j = i + 1; j < orphansList.size(); j++) {
            qreal currentWidth = orphansList[j]->getFrameRect(CoordType::zeroPoint).size().width();
            qreal maxWidth     = orphansList[nMax]->getFrameRect(CoordType::zeroPoint).size().width();
            if (currentWidth < maxWidth) {
                nMax = j;
            }
        }
        orphansList.swapItemsAt(i, nMax);
    }

    qreal x = 0, y = 0;
    qreal maxHeightInRow = 0.0;
    qreal groupMinWidth  = getGroupMinWidth();
    maxWidth             = qBound(10.0, groupMinWidth, 2000.0);

    for (auto currNode : orphansList) {
        QSizeF nodeSize = currNode->getNodeRect(CoordType::zeroPoint).size();

        if (x + nodeSize.width() > maxWidth) {
            y += maxHeightInRow + AppStyle::Sizes::orphansVerticalSpacer;
            //Переходим на следующий ряд
            maxHeightInRow = 0;
            x              = 0;
        }

        currNode->setPos(x, y);
        x += nodeSize.width() + AppStyle::Sizes::orphansHorizontalSpacer;

        maxHeightInRow = std::max(maxHeightInRow, nodeSize.height());
    }
}

void TermGroup::setTreeCoords()
{
    for (auto tree : trees) {
        tree->setTreeNodeCoors();
    }
}

QRectF TermGroup::getGroupRect() const
{
    return baseRect->getRect(CoordType::scene);
}
