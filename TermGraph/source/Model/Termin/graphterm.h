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

#pragma once

#include <QList>

#include "source/Model/Termin/infoterm.h"
#include "source/Model/TerminEdge/graphedge.h"

enum class NodeType { orphan, root, endLeaf, middleLeaf };

// This class contains all "graph" part of term
// like edges, place in graph hierarchy and so on
class GraphTerm : public InfoTerm
{
public:
    using List = QList<GraphTerm*>;

    GraphTerm(const NodeInfoContainer& info);
    ~GraphTerm() override = default;

    List            getUpDownNodes();
    GraphEdge::List getUpDownEdges();

    void addLayerNeighbours(GraphTerm::List neighboursList);
    void clearNeighboursList();

    bool isRoot() const;
    bool isOrphan() const;
    bool isLeaf() const;

    bool isInTree() const;

    bool hasConnections() const;

    NodeType getNodeType() const;

    QString getHierarchyDefinition();

    void setLevel(int level);
    int  getPaintLevel() const;
    void getCycleEdge();

    void         setTreeId(unsigned int treeId);
    unsigned int getTreeId() const;

    void        addEdgeRef(GraphEdge* edge);
    static bool isInGroupEdge(GraphEdge* edge);
    bool        hasConnectionsInGroup();

    // Delete section
    GraphTerm::List getRootNodes();
    GraphTerm::List getLeafNodes();

    GraphTerm::List getNeighbourNodes();
    GraphEdge::List getEdgesToLeafs() const;
    GraphEdge::List getEdgesToRoots() const;
    GraphEdge::List getAllConnectedEdges() const;

    int cycleSearchFlag = 0;

    void removeEdgeToLeafs(GraphEdge* edge);
    void removeEdgeToRoots(GraphEdge* edge);

    void            addBrokenEdge(GraphEdge* edge);
    GraphEdge::List getBrokenEdges() const;

    void checkForExceedEdges();

private:
    GraphTerm::List neighbourNodes;

    GraphEdge::List edgesToOtherGroups;  // Пока никак не используются но будут.
    GraphEdge::List brokenEdges;         // Пока никак не используются но будут.

    GraphEdge::List edgesToLeafs;
    GraphEdge::List edgesToRoots;

    int          paintLevel = -1;
    unsigned int treeId     = 0;  // 0 = no value

    static bool hasTermInRoots(GraphTerm* term, QList<GraphTerm*>& visitList);

    GraphEdge* findLongPathToNode(GraphTerm* node);

    void fillAllParentsList(GraphTerm* searchNode, GraphTerm::List& lst);
};
