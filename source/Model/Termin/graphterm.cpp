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

#include "source/Model/Termin/graphterm.h"

GraphTerm::GraphTerm(const NodeInfoContainer& info)
    : InfoTerm(info)
{}

GraphEdge::List GraphTerm::getUpDownEdges()
{
    GraphEdge::List ret;
    ret << edgesToLeafs;
    ret << edgesToRoots;
    return ret;
}

void GraphTerm::addLayerNeighbours(const GraphTerm::List& neighboursList)
{
    for (GraphTerm* term : neighboursList) {
        if (term == this) {
            continue;
        }
        neighbourNodes << term;
    }
}

void GraphTerm::clearNeighboursList()
{
    neighbourNodes.clear();
}

bool GraphTerm::isRoot() const
{
    return getNodeType() == NodeType::root;
}

bool GraphTerm::isOrphan() const
{
    return getNodeType() == NodeType::orphan;
}

bool GraphTerm::isLeaf() const
{
    NodeType type = getNodeType();
    return type == NodeType::endLeaf || type == NodeType::middleLeaf;
}

bool GraphTerm::isInTree() const
{
    return !isOrphan();
}

bool GraphTerm::hasConnections() const
{
    return !(edgesToRoots.isEmpty() && edgesToLeafs.isEmpty());
}

NodeType GraphTerm::getNodeType() const
{
    if (edgesToRoots.isEmpty()) {
        if (edgesToLeafs.isEmpty()) {
            return NodeType::orphan;  // Both empty
        } else {
            return NodeType::root;  // No connections down, has connections up
        }
    } else {
        if (edgesToLeafs.isEmpty()) {
            return NodeType::endLeaf;  // Has connections down, no connections up
        } else {
            return NodeType::middleLeaf;  // Has both connections, up and down
        }
    }
}

QString GraphTerm::getHierarchyDefinition()
{
    GraphTerm::List parentsList;
    fillAllParentsList(this, parentsList);

    if (parentsList.isEmpty()) {
        return "";
    }

    // Sorting parents list
    for (int i = 0; i < parentsList.size(); i++) {
        int maxIndex = i;
        for (int j = i + 1; j < parentsList.size(); j++) {
            if (parentsList[maxIndex]->paintLevel < parentsList[j]->paintLevel) {
                maxIndex = j;
            }
        }
        parentsList.swapItemsAt(i, maxIndex);
    }

    QStringList definitions;

    for (auto node : parentsList)
        definitions << node->getTermAndDefinition(true);

    // Add this definition
    definitions << getTermAndDefinition(true);

    return definitions.join("<br><br>");
}

void GraphTerm::setLevel(int level)
{
    if (level > paintLevel)
        paintLevel = level;

    for (auto n : getLeafNodes()) {
        if (getGroupUuid() != n->getGroupUuid()) {
            continue;
        }
        n->setLevel(level + 1);
    }
}

int GraphTerm::getPaintLevel() const
{
    return paintLevel;
}

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

unsigned int GraphTerm::getTreeId() const
{
    return treeId;
}

void GraphTerm::addEdgeRef(GraphEdge* edge)
{
    // We are source - connection up
    if (edge->getRoot() == this && edge->getLeaf() != this)
        edgesToLeafs << edge;

    // We are acceptor - connection down
    if (edge->getLeaf() == this && edge->getRoot() != this)
        edgesToRoots << edge;
}

bool GraphTerm::isInGroupEdge(GraphEdge* edge)
{
    return edge->getRoot()->getGroupUuid() == edge->getLeaf()->getGroupUuid();
}

bool GraphTerm::hasConnectionsInGroup()
{
    for (GraphEdge* e : edgesToLeafs) {
        if (e->getLeaf()->getGroupUuid() == getGroupUuid()) {
            return true;
        }
    }

    for (GraphEdge* e : edgesToRoots) {
        if (e->getLeaf()->getGroupUuid() == getGroupUuid()) {
            return true;
        }
    }
    return false;
}

GraphTerm::List GraphTerm::getUpDownNodes()
{
    List ret;
    ret << getRootNodes();
    ret << getLeafNodes();
    return ret;
}

GraphTerm::List GraphTerm::getRootNodes()
{
    GraphTerm::List ret;

    for (auto edge : edgesToRoots) {
        ret << edge->getOtherSide(this);
    }

    return ret;
}

GraphTerm::List GraphTerm::getLeafNodes()
{
    GraphTerm::List ret;

    for (auto edge : edgesToLeafs) {
        ret << edge->getOtherSide(this);
    }

    return ret;
}

GraphTerm::List GraphTerm::getNeighbourNodes()
{
    return neighbourNodes;
}

GraphEdge::List GraphTerm::getEdgesToLeafs() const
{
    return edgesToLeafs;
}

GraphEdge::List GraphTerm::getEdgesToRoots() const
{
    return edgesToRoots;
}

GraphEdge::List GraphTerm::getAllConnectedEdges() const
{
    GraphEdge::List ret;
    ret << edgesToRoots;
    ret << edgesToLeafs;
    return ret;
}

void GraphTerm::removeEdgeToLeafs(GraphEdge* edge)
{
    edgesToLeafs.removeOne(edge);
}

void GraphTerm::removeEdgeToRoots(GraphEdge* edge)
{
    edgesToRoots.removeOne(edge);
}

void GraphTerm::addBrokenEdge(GraphEdge* edge)
{
    if (!brokenEdges.contains(edge)) {
        brokenEdges << edge;
    }
}

GraphEdge::List GraphTerm::getBrokenEdges() const
{
    return brokenEdges;
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

void GraphTerm::fillAllParentsList(GraphTerm* searchNode, GraphTerm::List& lst)
{
    for (auto node : searchNode->getRootNodes()) {
        if (!lst.contains(node)) {
            lst << node;
        }

        fillAllParentsList(node, lst);
    }
}
