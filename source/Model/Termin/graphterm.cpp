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
        if (edge->data().needBroke) {
            continue;
        }

        auto leafNode = edge->oppositeTo(this);
        if (leafNode->cycleSearchFlag == 1) {
            edge->data().needBroke = true;
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
        edge->oppositeTo(this)->setTreeId(treeId);
    }
}

unsigned int GraphTerm::getTreeId() const { return treeId; }

void GraphTerm::addEdgeRef(GraphEdge::Ptr edge)
{
    // We are source - connection up
    if (edge->root().get() == this && edge->leaf().get() != this)
        edgesToLeafs.push_back(edge);

    // We are acceptor - connection down
    if (edge->leaf().get() == this && edge->root().get() != this)
        edgesToRoots.push_back(edge);
}

GraphTerm::List GraphTerm::getRootNodes()
{
    GraphTerm::List ret;

    for (auto edge : edgesToRoots) {
        ret.push_back(edge->oppositeTo(this));
    }

    return ret;
}

GraphTerm::List GraphTerm::getLeafNodes()
{
    GraphTerm::List ret;

    for (auto edge : edgesToLeafs) {
        ret.push_back(edge->oppositeTo(this));
    }

    return ret;
}

GraphEdge::List GraphTerm::getEdgesToLeafs() const { return edgesToLeafs; }

GraphEdge::List GraphTerm::getEdgesToRoots() const { return edgesToRoots; }

GraphEdge::List GraphTerm::getAllConnectedEdges() const
{
    GraphEdge::List ret;

    for (auto e : edgesToRoots)
        ret.push_back(e);

    for (auto e : edgesToLeafs)
        ret.push_back(e);

    return ret;
}

void GraphTerm::removeEdgeToLeafs(GraphEdge* edge)
{
    auto remIt = std::ranges::remove_if(edgesToLeafs, [edge](auto e) { return edge == e.get(); });
    edgesToLeafs.erase(remIt.begin(), remIt.end());
}

void GraphTerm::removeEdgeToRoots(GraphEdge* edge)
{
    auto remIt = std::ranges::remove_if(edgesToRoots, [edge](auto e) { return edge == e.get(); });
    edgesToRoots.erase(remIt.begin(), remIt.end());
}

void GraphTerm::addBrokenEdge(const GraphEdge::Ptr& edge)
{
    if (std::ranges::find(brokenEdges, edge) == brokenEdges.end())
        brokenEdges.push_back(edge);
}

void GraphTerm::addRedundantEdge(const GraphEdge::Ptr& edge)
{
    if (std::ranges::find(redundantEdges, edge) == redundantEdges.end())
        redundantEdges.push_back(edge);
}

void GraphTerm::checkForExceedEdges()
{
    for (auto node : getRootNodes()) {
        if (findLongPathToNode(node.get()) != nullptr) {
            // If we found long path, we need mark direct path for cut out
            for (auto edge : edgesToRoots) {
                if (edge->oppositeTo(this) == node) {
                    edge->data().needCutOut = true;
                }
            }
        }
    }
}

void GraphTerm::giveWeights()
{
    for (auto edge : getEdgesToRoots())
        edge->oppositeTo(this)->increaseWeight();

    for (auto edge : redundantEdges)
        edge->oppositeTo(this)->increaseWeight();
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

    for (auto rootNode : currentTerm->getRootNodes())
        if (!visitList.contains(rootNode.get()))
            visitList.push_back(rootNode.get());

    return hasTermInRoots(targetTerm, visitList);
}

GraphEdge* GraphTerm::findLongPathToNode(GraphTerm* node)
{
    for (auto edge : edgesToRoots) {
        auto otherSideNode = edge->oppositeTo(this).get();
        // Ignore direct connection
        if (otherSideNode == node)
            continue;

        QList<GraphTerm*> visitList;
        visitList.push_back(otherSideNode);

        if (hasTermInRoots(node, visitList)) {
            return edge.get();
        }
    }
    return nullptr;
}

void GraphTerm::fillAllParentsList(GraphTerm* searchNode, GraphTerm::UnsafeList& lst)
{
    for (auto node : searchNode->getRootNodes()) {
        if (std::ranges::find(lst, node.get()) == lst.end())
            lst.push_back(node.get());

        fillAllParentsList(node.get(), lst);
    }
}
