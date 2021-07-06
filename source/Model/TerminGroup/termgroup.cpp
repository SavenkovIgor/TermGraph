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

#include <ranges>

#include <QElapsedTimer>
#include <QThread>

#include "source/Helpers/appstyle.h"
#include "source/Helpers/fonts.h"
#include "source/Helpers/globaltagcache.h"
#include "source/Helpers/helpstuff.h"
#include "source/Helpers/link/linkutils.h"

TermGroup::TermGroup(const GroupData& info, const TermData::List& termData, QObject* parent)
    : QObject(parent)
    , mInfo(info)
{
    for (auto node : termData)
        Q_ASSERT_X(node.groupUuid == this->uuid(), Q_FUNC_INFO, "Node group error");

    mOrphansRect.setParentItem(&mBaseRect);

    QElapsedTimer t;
    t.start();

    PaintedTerm::List nodes;

    for (const auto& term : termData)
        nodes.push_back(std::make_shared<PaintedTerm>(term));

    mGraphData = GraphT({.nodes = nodes, .edges = searchAllConnections(nodes)});

    auto subgraphs = mGraphData.bondedSubgraphs();

    for (auto subgraph : subgraphs)
        mTrees.push_back(std::make_shared<PaintedForest>(subgraph));

    if (isThreadInterrupted())
        return;

    // Removing of exceed edges

    if (isThreadInterrupted())
        return;

    for (auto edge : allBrokenEdges())
        edge->brokeEdge();

    addTreeRectsToScene();

    addOrphansToParents();

    setTreeCoords();
    setOrphCoords();

    // Positioning
    updateRectsPositions();
    updateBaseRectSize();

    groupCreationTime = t.elapsed();
}

void TermGroup::setBasePoint(QPointF pt) { mBaseRect.setPos(pt); }

QRectF TermGroup::getGroupRect() const { return mBaseRect.getRect(CoordType::scene); }

UuidList TermGroup::searchNearest(const QString& text, int limit) const
{
    QString                        searchText = text.toLower();
    std::vector<QPair<int, QUuid>> searchResults;
    // Taking distances
    for (auto term : mGraphData.nodes) {
        auto lowerTerm = term->cache().lowerTerm();

        // Exact match
        if (searchText == lowerTerm) {
            searchResults.push_back({0, term->data().uuid});
            continue;
        }

        auto cuttedTerm = lowerTerm.left(searchText.size()); // Compare only left n characters

        auto acceptableLimit = static_cast<int>(cuttedTerm.size() * 0.25);
        auto distance        = LinkUtils::getLevDistance(cuttedTerm, searchText, acceptableLimit);

        if (distance <= acceptableLimit)
            searchResults.push_back({distance, term->data().uuid});
    }

    // Sorting
    std::ranges::sort(searchResults, [](auto s1, auto s2) { return s1.first < s2.first; });

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

    for (auto term : mGraphData.nodes) {
        if (term->cache().lowerTerm().contains(lowerSearch))
            ret.push_back(term->data().uuid);

        if (static_cast<int>(ret.size()) == limit)
            break;
    }

    return ret;
}

PaintedTerm::OptPtr TermGroup::getTerm(const QPointF& pt) const
{
    for (const auto tree : mTrees) {
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

    return std::nullopt;
}

PaintedTerm::OptPtr TermGroup::getTerm(const QUuid& termUuid) const
{
    for (auto term : mGraphData.nodes)
        if (term->data().uuid == termUuid)
            return term;

    return std::nullopt;
}

PaintedTerm::OptPtr TermGroup::getTerm(const QString& termName) const
{
    for (auto term : mGraphData.nodes)
        if (term->data().term == termName)
            return term;

    return std::nullopt;
}

void TermGroup::addOrphansToParents()
{
    for (auto node : getOrphanNodes()) {
        node->setParentItem(&mOrphansRect);
    }
}

qreal TermGroup::getGroupMinWidth()
{
    qreal width = 0.0;

    qreal groupNameWidth = getNameSize().width();
    qreal treeWidth      = getAllTreesSize().width();

    qreal orphansWidth = NodeVerticalStackTools::getNodeVerticalStackedSize(getOrphanNodes()).width();

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
    for (auto tree : mTrees) {
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

    width = std::max({width, nameSize.width(), treesSize.width(), orphansSize.width()});
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
    std::ranges::for_each(mTrees, [](auto t) { t->setTreeNodeCoords(); });
}

void TermGroup::setOrphCoords(qreal maxWidth)
{
    auto orphansList = getOrphanNodes();
    if (orphansList.empty()) {
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
        auto tmp          = orphansList[i];
        orphansList[i]    = orphansList[nMax];
        orphansList[nMax] = tmp;
    }

    QPointF pt(0, 0);
    qreal   maxHeightInRow = 0.0;
    qreal   groupMinWidth  = getGroupMinWidth();
    maxWidth               = qBound(10.0, groupMinWidth, 2000.0);

    for (auto currNode : orphansList) {
        QSizeF nodeSize = currNode->getNodeRect(CoordType::zeroPoint).size();

        if (pt.x() + nodeSize.width() > maxWidth) {
            pt.ry() += maxHeightInRow + AppStyle::Sizes::orphansVerticalSpacer;
            //Переходим на следующий ряд
            maxHeightInRow = 0;
            pt.rx()        = 0;
        }

        currNode->setPos(pt);
        pt.rx() += nodeSize.width() + AppStyle::Sizes::orphansHorizontalSpacer;

        maxHeightInRow = std::max(maxHeightInRow, nodeSize.height());
    }
}

void TermGroup::addTreeRectsToScene()
{
    for (auto tree : mTrees)
        tree->rect().setParentItem(&mBaseRect);
}

QSizeF TermGroup::getNameSize() const { return Fonts::getTextMetrics(name(), Fonts::getWeightFont()); }

QString TermGroup::qmlUuid() const { return uuid().toString(); }

PaintedTerm::List TermGroup::getRootNodes() const
{
    PaintedTerm::List ret;

    for (auto forest : mTrees)
        for (auto root : forest->roots())
            ret.push_back(root);

    return ret;
}

QSizeF TermGroup::getAllTreesSize()
{
    SizeList sizeList;

    for (const auto tree : mTrees)
        sizeList.push_back(tree->baseSize());

    auto totalSize = HelpStuff::getStackedSize(sizeList, Qt::Vertical);

    if (!mTrees.empty())
        totalSize.rheight() += (mTrees.size() - 1) * AppStyle::Sizes::groupVerticalSpacer;

    return totalSize;
}

PEdge::List TermGroup::searchAllConnections(const PaintedTerm::List& terms)
{
    PEdge::List ret;

    // Pre-heating of cache with exact terms match
    QMap<QString, PaintedTerm::Ptr> previousTagSearchCache = getExactTermMatchCache();
    QMap<QUuid, PaintedTerm::Ptr>   termUuids              = getTermUuidsMap();

    static int counter     = 0;
    bool       stopRequest = false;

    // Compare everything with everything
    for (auto node : terms) {
        for (const auto& link : node->cache().links()) {
            opt<PaintedTerm::Ptr> foundNode = std::nullopt;

            EdgeType eType = EdgeType::termin;

            if (!foundNode) {
                if (link.hasUuid()) {
                    if (termUuids.contains(link.uuid())) {
                        foundNode = termUuids[link.uuid()];
                        eType     = EdgeType::terminHardLink;
                    }
                }
            }

            // If we have same search earlier this cycle
            if (!foundNode && previousTagSearchCache.contains(link.textLower()))
                foundNode = previousTagSearchCache[link.textLower()];

            if (!foundNode)
                foundNode = getNearestNodeForTag(link.textLower(), terms);

            if (foundNode) {
                if (foundNode.value() != node) { // TODO: Real case, need check
                    ret.push_back(std::make_shared<PEdge>(foundNode.value(), node));
                    previousTagSearchCache.insert(link.textLower(), foundNode.value());
                }
            }

            counter++;
            if (counter % 20 == 0)
                if (isThreadInterrupted())
                    stopRequest = true;

            if (stopRequest)
                break;
        }
        if (stopRequest)
            break;
    }

    return ret;
}

opt<PaintedTerm::Ptr> TermGroup::getNearestNodeForTag(const QString& tag, const PaintedTerm::List& terms)
{
    opt<PaintedTerm::Ptr> targetTerm = std::nullopt;

    int minDistance = 100000;

    opt<int> optionalResult;

    for (auto node : terms) {
        auto termName = node->cache().lowerTerm();

        if (!LinkUtils::tagLengthSuitTerm(tag, termName))
            continue;

        auto cacheMatch = GlobalTagCache::instance().get(tag, termName);
        if (cacheMatch) {
            optionalResult = cacheMatch.value();
        } else {
            optionalResult = LinkUtils::getDistanceBetweenTagAndTerm(tag, termName, minDistance);
            GlobalTagCache::instance().add(tag, termName, optionalResult);
        }

        if (optionalResult) {
            if (optionalResult.value() == 0) // Already best match, no need to count further
                return node;

            if (optionalResult.value() < minDistance) {
                minDistance = optionalResult.value();
                targetTerm  = node;
            }
        }
    }

    return targetTerm;
}

PEdge::List TermGroup::filterFromEdgesList(std::function<bool(PEdge::Ptr)> condition) const
{
    PEdge::List ret;

    for (auto edge : allEdges()) {
        if (condition(edge)) {
            ret.push_back(edge);
        }
    }

    return ret;
}

PEdge::List TermGroup::edgesForPaint() const
{
    PEdge::List lst;

    auto softEdgesFilter     = [](auto e) { return !e->isSelected() && !e->isHard(); };
    auto hardEdgesFilter     = [](auto e) { return !e->isSelected() && e->isHard(); };
    auto selectedEdgesFilter = [](auto e) { return e->isSelected(); };

    auto softEdges     = filterFromEdgesList(softEdgesFilter);
    auto hardEdges     = filterFromEdgesList(hardEdgesFilter);
    auto selectedEdges = filterFromEdgesList(selectedEdgesFilter);

    auto addToList = [&lst](auto e) { lst.push_back(e); };

    std::ranges::for_each(softEdges, addToList);
    std::ranges::for_each(hardEdges, addToList);
    std::ranges::for_each(selectedEdges, addToList);
    std::ranges::for_each(allBrokenEdges(), addToList);

    return lst;
}

QUuid TermGroup::uuid() const { return mInfo.uuid; }

QString TermGroup::name() const { return mInfo.name; }

QString TermGroup::getHierarchyDefinition(PaintedTerm::Ptr term)
{
    for (auto tree : mTrees) {
        if (tree->contains(term))
            return tree->getHierarchyDefinition(term);
    }

    return "";
}

void TermGroup::selectTerm(const PaintedTerm::Ptr& term, bool selection)
{
    for (auto forest : mTrees)
        if (forest->hasTerm(term))
            forest->selectTerm(term, selection);
}

NodeType TermGroup::termType(const PaintedTerm::Ptr& term) const
{
    for (auto forest : mTrees) {
        if (forest->hasTerm(term))
            return forest->nodeType(term);
    }

    return NodeType::orphan;
}

QMap<QString, PaintedTerm::Ptr> TermGroup::getExactTermMatchCache()
{
    QMap<QString, PaintedTerm::Ptr> ret;

    for (auto node : mGraphData.nodes)
        ret.insert(node->cache().lowerTerm(), node);

    return ret;
}

QMap<QUuid, PaintedTerm::Ptr> TermGroup::getTermUuidsMap()
{
    QMap<QUuid, PaintedTerm::Ptr> ret;

    for (auto node : mGraphData.nodes)
        ret.insert(node->data().uuid, node);

    return ret;
}

QSizeF TermGroup::getOrphansSize()
{
    QRectF orphansRc;
    for (auto node : getOrphanNodes()) {
        orphansRc = orphansRc.united(node->getNodeRect(CoordType::scene));
    }
    return orphansRc.size();
}

PaintedTerm::List TermGroup::getInTreeNodes() const { return mGraphData.connectedNodes(); }

PaintedTerm::List TermGroup::getOrphanNodes() const { return mGraphData.isolatedNodes(); }

PaintedTerm::List TermGroup::filterFromNodesList(std::function<bool(PaintedTerm::Ptr)> filterCheck) const
{
    PaintedTerm::List ret;
    for (auto node : mGraphData.nodes) {
        if (filterCheck(node)) {
            ret.push_back(node);
        }
    }
    return ret;
}

PEdge::List TermGroup::allEdges() const
{
    PEdge::List ret;

    for (auto forest : mTrees) {
        for (auto edge : forest->edgeList()) {
            ret.push_back(edge);
        }
    }

    return ret;
}

PEdge::List TermGroup::allBrokenEdges() const
{
    PEdge::List ret;
    for (auto forest : mTrees) {
        for (auto edge : forest->brokenEdges()) {
            ret.push_back(edge);
        }
    }

    return ret;
}

PEdge::List TermGroup::allExceedEdges() const
{
    PEdge::List ret;

    for (auto forest : mTrees) {
        for (auto edge : forest->wasteEdges())
            ret.push_back(edge);
    }

    return ret;
}

PaintedTerm::List TermGroup::terms() const { return mGraphData.nodes; }

bool TermGroup::isThreadInterrupted() { return QThread::currentThread()->isInterruptionRequested(); }
