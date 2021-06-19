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

#include "source/Model/Termin/graphterm.h"

#include <ranges>

int GraphTerm::mMaxWeight = 0;

GraphTerm::GraphTerm(const TermData& info)
    : Node<TermData>(info)
    , mDataCache(info)
{}

bool GraphTerm::isRoot() const { return getNodeType() == NodeType::root; }

bool GraphTerm::isOrphan() const { return getNodeType() == NodeType::orphan; }

bool GraphTerm::isLeaf() const
{
    NodeType type = getNodeType();
    return type == NodeType::endLeaf || type == NodeType::middleLeaf;
}

bool GraphTerm::isInTree() const { return !isOrphan(); }

NodeType GraphTerm::getNodeType() const
{
    if (edgesToRoots.empty()) {
        if (edgesToLeafs.empty()) {
            return NodeType::orphan; // Both empty
        } else {
            return NodeType::root; // No connections down, has connections up
        }
    } else {
        if (edgesToLeafs.empty()) {
            return NodeType::endLeaf; // Has connections down, no connections up
        } else {
            return NodeType::middleLeaf; // Has both connections, up and down
        }
    }
}

QString GraphTerm::getHierarchyDefinition()
{
    GraphTerm::UnsafeList parentsList;
    fillAllParentsList(this, parentsList);

    if (parentsList.empty())
        return "";

    // Sorting parents list
    for (int i = 0; i < parentsList.size(); i++) {
        int maxIndex = i;
        for (int j = i + 1; j < parentsList.size(); j++) {
            if (parentsList[maxIndex]->paintLevel < parentsList[j]->paintLevel) {
                maxIndex = j;
            }
        }
        auto* tmp             = parentsList[i];
        parentsList[i]        = parentsList[maxIndex];
        parentsList[maxIndex] = tmp;
    }

    QStringList definitions;

    for (auto node : parentsList)
        definitions << node->cache().termAndDefinition(true);

    // Add this definition
    definitions << cache().termAndDefinition(true);

    return definitions.join("<br><br>");
}

void GraphTerm::setLevel(int level)
{
    if (level > paintLevel)
        paintLevel = level;

    for (auto node : getLeafNodes()) {
        if (data().groupUuid != node->data().groupUuid) {
            continue;
        }
        node->setLevel(level + 1);
    }
}

int GraphTerm::getPaintLevel() const { return paintLevel; }

void GraphTerm::getCycleEdge()
{
    cycleSearchFlag = 1;
    for (auto edge : edgesToLeafs) {
        if (edge->needBroke) {
            continue;
        }

        auto leafNode = edge->getOtherSide(this);
        if (leafNode->cycleSearchFlag == 1) {
            edge->needBroke = true;
        } else {
            if (leafNode->cycleSearchFlag == 0) {
                leafNode->getCycleEdge();
            }
        }
    }
    cycleSearchFlag = 2;
}

void GraphTerm::setTreeId(unsigned int treeId)
{
    if (this->treeId == 0) {
        this->treeId = treeId;
    } else {
        return;
    }

    for (auto edge : getAllConnectedEdges()) {
        edge->getOtherSide(this)->setTreeId(treeId);
    }
}

unsigned int GraphTerm::getTreeId() const { return treeId; }

void GraphTerm::addEdgeRef(GraphEdge* edge)
{
    // We are source - connection up
    if (edge->getRoot() == this && edge->getLeaf() != this)
        edgesToLeafs.push_back(edge);

    // We are acceptor - connection down
    if (edge->getLeaf() == this && edge->getRoot() != this)
        edgesToRoots.push_back(edge);
}

GraphTerm::UnsafeList GraphTerm::getRootNodes()
{
    GraphTerm::UnsafeList ret;

    for (auto edge : edgesToRoots) {
        ret.push_back(edge->getOtherSide(this));
    }

    return ret;
}

GraphTerm::UnsafeList GraphTerm::getLeafNodes()
{
    GraphTerm::UnsafeList ret;

    for (auto edge : edgesToLeafs) {
        ret.push_back(edge->getOtherSide(this));
    }

    return ret;
}

GraphEdge::UnsafeList GraphTerm::getEdgesToLeafs() const { return edgesToLeafs; }

GraphEdge::UnsafeList GraphTerm::getEdgesToRoots() const { return edgesToRoots; }

GraphEdge::UnsafeList GraphTerm::getAllConnectedEdges() const
{
    GraphEdge::UnsafeList ret;

    for (auto e : edgesToRoots)
        ret.push_back(e);

    for (auto e : edgesToLeafs)
        ret.push_back(e);

    return ret;
}

void GraphTerm::removeEdgeToLeafs(GraphEdge* edge)
{
    auto remIt = std::ranges::remove_if(edgesToLeafs, [edge](auto e) { return edge == e; });
    edgesToLeafs.erase(remIt.begin(), remIt.end());
}

void GraphTerm::removeEdgeToRoots(GraphEdge* edge)
{
    auto remIt = std::ranges::remove_if(edgesToRoots, [edge](auto e) { return edge == e; });
    edgesToRoots.erase(remIt.begin(), remIt.end());
}

void GraphTerm::addBrokenEdge(GraphEdge* edge)
{
    if (std::ranges::find(brokenEdges, edge) == brokenEdges.end())
        brokenEdges.push_back(edge);
}

void GraphTerm::addRedundantEdge(GraphEdge* edge)
{
    if (std::ranges::find(redundantEdges, edge) == redundantEdges.end())
        redundantEdges.push_back(edge);
}

void GraphTerm::checkForExceedEdges()
{
    for (auto node : getRootNodes()) {
        if (findLongPathToNode(node) != nullptr) {
            // If we found long path, we need mark direct path for cut out
            for (auto edge : edgesToRoots) {
                if (edge->getOtherSide(this) == node) {
                    edge->needCutOut = true;
                }
            }
        }
    }
}

void GraphTerm::giveWeights()
{
    for (auto* edge : getEdgesToRoots())
        edge->getOtherSide(this)->increaseWeight();

    for (auto* edge : redundantEdges)
        edge->getOtherSide(this)->increaseWeight();
}

void GraphTerm::resetMaxWeight() { mMaxWeight = 0; }

int GraphTerm::weight() const { return mSelfWeight; }

void GraphTerm::increaseWeight()
{
    mSelfWeight++;
    mMaxWeight = std::max(mMaxWeight, mSelfWeight);
}

double GraphTerm::getRelativeWeight() const { return static_cast<double>(weight()) / mMaxWeight; }

const TermDataCache& GraphTerm::cache() const { return mDataCache; }

bool GraphTerm::hasTermInRoots(GraphTerm* targetTerm, QList<GraphTerm*>& visitList)
{
    if (visitList.isEmpty())
        return false;

    auto* currentTerm = visitList.takeFirst();

    if (targetTerm == currentTerm)
        return true;

    for (auto* rootNode : currentTerm->getRootNodes())
        if (!visitList.contains(rootNode))
            visitList.push_back(rootNode);

    return hasTermInRoots(targetTerm, visitList);
}

GraphEdge* GraphTerm::findLongPathToNode(GraphTerm* node)
{
    for (auto edge : edgesToRoots) {
        auto otherSideNode = edge->getOtherSide(this);
        // Ignore direct connection
        if (otherSideNode == node)
            continue;

        QList<GraphTerm*> visitList;
        visitList.push_back(otherSideNode);

        if (hasTermInRoots(node, visitList)) {
            return edge;
        }
    }
    return nullptr;
}

void GraphTerm::fillAllParentsList(GraphTerm* searchNode, GraphTerm::UnsafeList& lst)
{
    for (auto node : searchNode->getRootNodes()) {
        if (std::ranges::find(lst, node) == lst.end())
            lst.push_back(node);

        fillAllParentsList(node, lst);
    }
}
