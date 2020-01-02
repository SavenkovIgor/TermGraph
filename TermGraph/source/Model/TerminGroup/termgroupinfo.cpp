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

#include "termgroupinfo.h"

#include "source/Helpers/helpstuff.h"
#include "source/Helpers/globaltagcache.h"
#include "source/Model/TerminGroup/groupnamecache.h"

TermGroupInfo::TermGroupInfo(const GroupInfoContainer& info)
{
    this->info = info;
}

TermGroupInfo::~TermGroupInfo()
{
    for (auto tree : trees)
        delete tree;

    for (auto node : getAllNodes())
        delete node;

    for (auto edge : getAllEdges())
        delete edge;
}

QUuid TermGroupInfo::getUuid() const
{
    return info.uuid;
}

QString TermGroupInfo::getName() const
{
    return info.name;
}

PaintedTerm::List TermGroupInfo::getAllNodes() const
{
    return nodesList;
}

EdgesList TermGroupInfo::getAllEdges() const
{
    return edgesList;
}

EdgesList TermGroupInfo::filterFromEdgesList(std::function<bool(Edge*)> condition) const
{
    EdgesList ret;
    for (auto* edge : getAllEdges()) {
        if (condition(edge)) {
            ret << edge;
        }
    }

    return ret;
}

EdgesList TermGroupInfo::getBrokenEdges() const
{
    EdgesList ret;
    for (auto* node : getAllNodes())
        for (auto* edge : node->getBrokenEdges())
            ret.push_back(dynamic_cast<Edge*>(edge));

    return ret;
}

EdgesList TermGroupInfo::getAllEdgesForPainting() const
{
    EdgesList lst;
    auto      defaultTypeFilter  = [](Edge* e) { return e->selectedType() == EdgeSelected::none; };
    auto      selectedTypeFilter = [](Edge* e) { return e->selectedType() != EdgeSelected::none; };

    lst << filterFromEdgesList(defaultTypeFilter);
    lst << filterFromEdgesList(selectedTypeFilter);
    lst << getBrokenEdges();
    return lst;
}

PaintedTerm::List TermGroupInfo::getRootNodes() const
{
    return filterFromNodesList([] (PaintedTerm* node) { return node->isRoot(); });
}

PaintedTerm::List TermGroupInfo::getInTreeNodes() const
{
    return filterFromNodesList([] (PaintedTerm* node) { return node->isInTree(); });
}

PaintedTerm::List TermGroupInfo::getOrphanNodes() const
{
    return filterFromNodesList([] (PaintedTerm* node) { return node->isOrphan(); } );
}

Edge* TermGroupInfo::addNewEdge(PaintedTerm* rootNode, PaintedTerm* leafNode)
{
    if (rootNode == leafNode)
        return nullptr;

    auto edge = new Edge(rootNode, leafNode);
    rootNode->addEdgeRef(edge);
    leafNode->addEdgeRef(edge);
    return edge;
}

EdgesList TermGroupInfo::searchAllConnections()
{
    EdgesList ret;

    // Compare everything with everything
    for (auto node : nodesList) {
        for (const auto& tag : node->getDefinitionTags()) {
            if (auto* foundNode = getNearestNodeForTag(tag)) {
                if (auto* edge = addNewEdge(foundNode, node)) {
                    ret << edge;
                }
            }
        }
    }

    return ret;
}

PaintedTerm* TermGroupInfo::getNearestNodeForTag(const QString& tag)
{
    PaintedTerm* targetTerm = nullptr;

    int minDistance = 100000;

    std::optional<int> optionalResult;

    for (auto node : nodesList) {
        auto termName = node->getCachedLowerTerm();

        if (!TagProcessor::tagLengthSuitTerm(tag, termName))
            continue;

        auto cacheMatch = GlobalTagCache::instance().get(tag, termName);
        if (cacheMatch) {
            optionalResult = cacheMatch.value();
        } else {
            optionalResult = TagProcessor::getDistanceBetweenTagAndTerm(tag, termName);
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

void TermGroupInfo::loadEdges()
{
    edgesList << searchAllConnections();
}

void TermGroupInfo::removeCycles()
{
    // First find all edges to break
    for (auto node : nodesList) {
        node->getCycleEdge();
    }

    EdgesList brokeList;
    for (auto edge : edgesList) {
        if (edge->needBroke) {
            brokeList << edge;
        }
    }

    for (auto edge : brokeList) {
        edge->brokeEdge();
        edgesList.removeOne(edge);
    }
}

void TermGroupInfo::removeExceedEdges()
{
    // First find all edges to break
    for (auto node : nodesList) {
        node->checkForExceedEdges();
    }

    EdgesList brokeList;
    for (auto edge : edgesList) {
        if (edge->needCutOut) {
            brokeList << edge;
        }
    }

    for (auto edge : brokeList) {
        edge->cutOutFromSides();
        edgesList.removeOne(edge);
        delete edge;
    }
}

void TermGroupInfo::setLevels()
{
    // Set layer numbers
    for (auto node : getRootNodes()) {
        node->setLevel(0);
    }
}

void TermGroupInfo::initTrees()
{
    for (auto tree : trees) {
        delete tree;
    }
    trees.clear();
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
        auto tree = new TermTree();
        for (auto node : treeNodes) {
            if (node->getTreeId() == treeId) {
                tree->addTerm(node);
            }
        }
        trees.push_back(tree);
    }
}

QSizeF TermGroupInfo::getAllTreesSize()
{
    SizesList sizeList;

    for (const auto& tree : trees)
        sizeList.push_back(tree->getTreeSize());

    auto totalSize = HelpStuff::getStackedSize(sizeList, Qt::Vertical);

    if (!trees.isEmpty()) {
        totalSize.rheight() += (trees.size() - 1)*AppStyle::Sizes::groupVerticalSpacer;
    }

    return totalSize;
}

void TermGroupInfo::setNeighbours()
{
    for (auto tree : trees) {
        tree->setNeighbours();
    }
}

QSizeF TermGroupInfo::getOrphansSize()
{
    QRectF orphansRc;
    for (auto node : getOrphanNodes()) {
        orphansRc = orphansRc.united(node ->getNodeRect(CoordType::scene));
    }
    return orphansRc.size();
}

PaintedTerm::List TermGroupInfo::filterFromNodesList(std::function<bool(PaintedTerm*)> filterCheck) const
{
    PaintedTerm::List ret;
    for (auto n : nodesList) {
        if (filterCheck(n)) {
            ret << n;
        }
    }
    return ret;
}

void TermGroupInfo::addNodeToList(PaintedTerm *node)
{
    nodesList << node;
}

void TermGroupInfo::clearNodesList()
{
    nodesList.clear();
}
