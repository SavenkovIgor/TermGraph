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

#include "source/Model/TerminGroup/termgroupinfo.h"

#include <QThread>

#include "source/Helpers/appstyle.h"
#include "source/Helpers/globaltagcache.h"
#include "source/Helpers/handytypes.h"
#include "source/Helpers/helpstuff.h"
#include "source/Helpers/link/linkutils.h"
#include "source/Model/TerminGroup/groupnamecache.h"

TermGroupInfo::TermGroupInfo(const GroupData& info, const PaintedTerm::List& nodes)
    : mInfo(info)
    , mNodes(nodes)
{
    for (auto* node : nodes)
        Q_ASSERT_X(node->data().groupUuid == this->uuid(), Q_FUNC_INFO, "Node group error");
}

TermGroupInfo::~TermGroupInfo()
{
    removeTrees();
    qDeleteAll(mNodes);
    qDeleteAll(mEdges);
}

QUuid TermGroupInfo::uuid() const { return mInfo.uuid; }

QString TermGroupInfo::name() const { return mInfo.name; }

PaintedTerm::List TermGroupInfo::nodes() const { return mNodes; }

PaintedEdge::List TermGroupInfo::edges() const { return mEdges; }

PaintedEdge::List TermGroupInfo::brokenEdges() const
{
    PaintedEdge::List ret;
    for (auto* node : nodes())
        for (auto* edge : node->getBrokenEdges())
            ret.push_back(dynamic_cast<PaintedEdge*>(edge));

    return ret;
}

PaintedEdge::List TermGroupInfo::redundantEdges() const
{
    PaintedEdge::List ret;
    for (auto* node : nodes())
        for (auto* edge : node->getRedundantEdges())
            ret.push_back(dynamic_cast<PaintedEdge*>(edge));

    return ret;
}

PaintedEdge::List TermGroupInfo::edgesForPaint() const
{
    PaintedEdge::List lst;

    auto softEdgesFilter     = [](PaintedEdge* e) { return !e->isSelected() && !e->isHard(); };
    auto hardEdgesFilter     = [](PaintedEdge* e) { return !e->isSelected() && e->isHard(); };
    auto selectedEdgesFilter = [](PaintedEdge* e) { return e->isSelected(); };

    auto softEdges     = filterFromEdgesList(softEdgesFilter);
    auto hardEdges     = filterFromEdgesList(hardEdgesFilter);
    auto selectedEdges = filterFromEdgesList(selectedEdgesFilter);

    lst << softEdges;
    lst << hardEdges;
    lst << selectedEdges;
    lst << brokenEdges();

    return lst;
}

PaintedTerm::List TermGroupInfo::getRootNodes() const
{
    return filterFromNodesList([](PaintedTerm* node) { return node->isRoot(); });
}

PaintedTerm::List TermGroupInfo::getInTreeNodes() const
{
    return filterFromNodesList([](PaintedTerm* node) { return node->isInTree(); });
}

PaintedTerm::List TermGroupInfo::getOrphanNodes() const
{
    return filterFromNodesList([](PaintedTerm* node) { return node->isOrphan(); });
}

PaintedEdge::List TermGroupInfo::searchAllConnections()
{
    PaintedEdge::List ret;

    // Pre-heating of cache with exact terms match
    QMap<QString, PaintedTerm*> previousTagSearchCache = getExactTermMatchCache();
    QMap<QUuid, PaintedTerm*>   termUuids              = getTermUuidsMap();

    static int counter     = 0;
    bool       stopRequest = false;

    // Compare everything with everything
    for (auto* node : mNodes) {
        for (const auto& link : node->cache().links()) {
            PaintedTerm* foundNode = nullptr;

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
            if (!foundNode)
                foundNode = previousTagSearchCache.value(link.textLower(), nullptr);

            if (!foundNode)
                foundNode = getNearestNodeForTag(link.textLower());

            if (foundNode) {
                if (foundNode != node) { // TODO: Real case, need check
                    ret << new PaintedEdge(foundNode, node, eType);
                    previousTagSearchCache.insert(link.textLower(), foundNode);
                }
            }

            counter++;
            if (counter % 20 == 0)
                if (buildingWasInterrupted())
                    stopRequest = true;

            if (stopRequest)
                break;
        }
        if (stopRequest)
            break;
    }

    return ret;
}

QMap<QString, PaintedTerm*> TermGroupInfo::getExactTermMatchCache()
{
    QMap<QString, PaintedTerm*> ret;

    for (auto* node : mNodes)
        ret.insert(node->cache().lowerTerm(), node);

    return ret;
}

QMap<QUuid, PaintedTerm*> TermGroupInfo::getTermUuidsMap()
{
    QMap<QUuid, PaintedTerm*> ret;

    for (auto* node : mNodes)
        ret.insert(node->data().uuid, node);

    return ret;
}

PaintedTerm* TermGroupInfo::getNearestNodeForTag(const QString& tag)
{
    PaintedTerm* targetTerm = nullptr;

    int minDistance = 100000;

    opt<int> optionalResult;

    for (auto* node : mNodes) {
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

void TermGroupInfo::removeTrees()
{
    qDeleteAll(mTrees);
    mTrees.clear();
}

bool TermGroupInfo::buildingWasInterrupted() { return QThread::currentThread()->isInterruptionRequested(); }

void TermGroupInfo::loadEdges() { mEdges << searchAllConnections(); }

void TermGroupInfo::removeCycles()
{
    // First find all edges to break
    for (auto* node : mNodes) {
        node->getCycleEdge();
    }

    PaintedEdge::List brokeList;
    for (auto* edge : mEdges) {
        if (edge->needBroke) {
            brokeList << edge;
        }
    }

    for (auto edge : brokeList) {
        edge->brokeEdge();
        mEdges.removeOne(edge);
    }
}

void TermGroupInfo::removeExceedEdges()
{
    // First find all edges to break
    for (auto* node : mNodes)
        node->checkForExceedEdges();

    PaintedEdge::List brokeList;
    for (auto* edge : mEdges) {
        if (edge->needCutOut) {
            brokeList << edge;
        }
    }

    for (auto edge : brokeList) {
        edge->makeEdgeRedundant();
        mEdges.removeOne(edge);
    }
}

void TermGroupInfo::setLevels()
{
    // Set layer numbers
    for (auto node : getRootNodes())
        node->setLevel(0);
}

void TermGroupInfo::initTrees()
{
    removeTrees();

    unsigned int treeId = 1;

    auto treeNodes = getInTreeNodes();

    // Set all tree Id's
    for (auto node : treeNodes) {
        if (node->getTreeId() == 0) {
            node->setTreeId(treeId);
            treeId++;
        }
    }

    unsigned int treesCount = treeId - 1; // last treeId increase was fictious

    // Set all trees
    for (unsigned int treeId = 1; treeId <= treesCount; treeId++) {
        auto* tree = new TermTree();
        for (auto node : treeNodes) {
            if (node->getTreeId() == treeId) {
                tree->addTerm(node);
            }
        }

        mTrees.push_back(tree);
    }

    auto treeSorting = [](const TermTree* t1, const TermTree* t2) { return t1->square() > t2->square(); };

    std::sort(mTrees.begin(), mTrees.end(), treeSorting);
}

QSizeF TermGroupInfo::getAllTreesSize()
{
    SizeList sizeList;

    for (const auto* tree : mTrees)
        sizeList.push_back(tree->baseSize());

    auto totalSize = HelpStuff::getStackedSize(sizeList, Qt::Vertical);

    if (!mTrees.empty())
        totalSize.rheight() += (mTrees.size() - 1) * AppStyle::Sizes::groupVerticalSpacer;

    return totalSize;
}

QSizeF TermGroupInfo::getOrphansSize()
{
    QRectF orphansRc;
    for (auto node : getOrphanNodes()) {
        orphansRc = orphansRc.united(node->getNodeRect(CoordType::scene));
    }
    return orphansRc.size();
}

TermTree::List TermGroupInfo::trees() const { return mTrees; }

PaintedTerm::List TermGroupInfo::filterFromNodesList(std::function<bool(PaintedTerm*)> filterCheck) const
{
    PaintedTerm::List ret;
    for (auto* node : mNodes) {
        if (filterCheck(node)) {
            ret << node;
        }
    }
    return ret;
}

PaintedEdge::List TermGroupInfo::filterFromEdgesList(std::function<bool(PaintedEdge*)> condition) const
{
    PaintedEdge::List ret;
    for (auto* edge : edges()) {
        if (condition(edge)) {
            ret << edge;
        }
    }

    return ret;
}
