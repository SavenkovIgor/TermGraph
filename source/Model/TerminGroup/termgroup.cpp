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

#include "source/Model/TerminGroup/termgroup.h"

#include <QElapsedTimer>

#include "source/Helpers/appstyle.h"
#include "source/Helpers/fonts.h"
#include "source/Helpers/link/linkutils.h"

TermGroup::TermGroup(const GroupData& info, QObject* parent)
    : QObject(parent)
    , TermGroupInfo(info)
{
    mOrphansRect.setParentItem(&mBaseRect);
}

void TermGroup::loadNodes(const PaintedTerm::List& newNodes)
{
    clearNodesList();

    for (auto* node : newNodes) {
        Q_ASSERT_X(node->data().groupUuid == this->uuid(), Q_FUNC_INFO, "Node group error");

        addNodeToList(node);
    }

    initNewNodes();
}

void TermGroup::setBasePoint(QPointF pt) { mBaseRect.setPos(pt); }

QRectF TermGroup::getGroupRect() const { return mBaseRect.getRect(CoordType::scene); }

UuidList TermGroup::searchNearest(const QString& text, int limit) const
{
    QString                  searchText = text.toLower();
    QList<QPair<int, QUuid>> searchResults;
    // Taking distances
    for (auto* node : nodes()) {
        auto lowerTerm = node->cache().lowerTerm();

        // Exact match
        if (searchText == lowerTerm) {
            searchResults << QPair<int, QUuid>(0, node->data().uuid);
            continue;
        }

        auto cuttedTerm = lowerTerm.left(searchText.size()); // Compare only left n characters

        auto acceptableLimit = static_cast<int>(cuttedTerm.size() * 0.25);
        auto distance        = LinkUtils::getLevDistance(cuttedTerm, searchText, acceptableLimit);

        if (distance <= acceptableLimit)
            searchResults << QPair<int, QUuid>(distance, node->data().uuid);
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

UuidList TermGroup::searchContains(const QString& text, int limit) const
{
    UuidList ret;
    auto     lowerSearch = text.toLower();

    for (auto* node : nodes()) {
        if (node->cache().lowerTerm().contains(lowerSearch))
            ret.push_back(node->data().uuid);

        if (static_cast<int>(ret.size()) == limit)
            break;
    }

    return ret;
}

PaintedTerm* TermGroup::getNode(const QPointF& pt) const
{
    for (const auto* tree : trees()) {
        if (tree->getTreeRect(CoordType::scene).contains(pt)) {
            return tree->getNodeAtPoint(pt);
        }
    }

    auto orphansRect = QRectF(this->mOrphansRect.scenePos(), this->mOrphansRect.getSize());
    if (orphansRect.contains(pt)) {
        for (auto orphan : getOrphanNodes()) {
            if (orphan->getNodeRect(CoordType::scene).contains(pt)) {
                return orphan;
            }
        }
    }

    return nullptr;
}

PaintedTerm* TermGroup::getNode(const QUuid& nodeUuid) const
{
    for (auto* node : nodes())
        if (node->data().uuid == nodeUuid)
            return node;

    return nullptr;
}

PaintedTerm* TermGroup::getNode(const QString& nodeName) const
{
    for (auto* node : nodes())
        if (node->data().term == nodeName)
            return node;

    return nullptr;
}

void TermGroup::initNewNodes()
{
    QElapsedTimer t;
    t.start();

    loadEdges();
    setParentForNodesAndEdges();

    if (buildingWasInterrupted())
        return;

    removeCycles();
    removeExceedEdges();

    if (buildingWasInterrupted())
        return;

    setLevels();
    initTrees();
    addTreeRectsToScene();

    addOrphansToParents();
    addEdgesToParents();

    setTreeCoords();
    setOrphCoords();

    setAllWeights();

    // Positioning
    updateRectsPositions();
    updateBaseRectSize();

    groupCreationTime = t.elapsed();
}

void TermGroup::setParentForNodesAndEdges()
{
    for (auto* node : nodes())
        node->setParent(this);

    for (auto* edge : edges())
        edge->setParent(this);
}

void TermGroup::addOrphansToParents()
{
    for (auto node : getOrphanNodes()) {
        node->setParentItem(&mOrphansRect);
    }
}

void TermGroup::addEdgesToParents()
{
    for (Edge* edge : edges()) {
        for (auto* tree : trees()) {
            if (tree->hasEdge(edge)) {
                edge->setParentItem(&(tree->rect()));
            }
        }
    }
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

void TermGroup::updateRectsPositions()
{
    qreal   vSpacer = AppStyle::Sizes::groupVerticalSpacer;
    qreal   hSpacer = AppStyle::Sizes::groupHorizontalSpacer;
    QPointF basePoint(QPointF(hSpacer, vSpacer));

    auto nameSize = getNameSize();

    // Устанавливаем базовую точку имени
    basePoint.ry() += nameSize.height() + vSpacer;

    // Вычисляем под дерево
    for (auto* tree : trees()) {
        auto treeSize = tree->baseSize();
        tree->rect().setPos(basePoint);
        tree->rect().setSize(treeSize);
        basePoint.ry() += treeSize.height() + vSpacer;
    }

    QSizeF orphansSize = getOrphansSize();
    // Вычисляем под несвязанные вершины
    mOrphansRect.setPos(basePoint);
    mOrphansRect.setSize(orphansSize); // Применяем
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

    mBaseRect.setSize(QSizeF(width, height));
}

void TermGroup::setTreeCoords()
{
    for (auto* tree : trees())
        tree->setTreeNodeCoords();
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

void TermGroup::setAllWeights()
{
    GraphTerm::resetMaxWeight();

    for (auto* node : nodes())
        node->giveWeights();
}

void TermGroup::addTreeRectsToScene()
{
    for (auto* tree : trees())
        tree->rect().setParentItem(&mBaseRect);
}

QSizeF TermGroup::getNameSize() const { return Fonts::getTextMetrics(name(), Fonts::getWeightFont()); }

QString TermGroup::qmlUuid() const { return uuid().toString(); }
