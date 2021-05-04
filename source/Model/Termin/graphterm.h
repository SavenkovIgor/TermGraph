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

#pragma once

#include <QList>

#include "source/Model/Base/node.hpp"
#include "source/Model/Termin/termdatacache.h"
#include "source/Model/TerminEdge/graphedge.h"

enum class NodeType { orphan, root, endLeaf, middleLeaf };

// This class contains all "graph" part of term
// like edges, place in graph hierarchy and so on
class GraphTerm : public Node<TermData>
{
public:
    using List = QList<GraphTerm*>;

    GraphTerm(const TermData& info);
    ~GraphTerm() = default;

    bool isRoot() const;
    bool isOrphan() const;
    bool isLeaf() const;

    bool isInTree() const;

    NodeType getNodeType() const;

    QString getHierarchyDefinition();

    void setLevel(int level);
    int  getPaintLevel() const;
    void getCycleEdge();

    void         setTreeId(unsigned int treeId);
    unsigned int getTreeId() const;

    void addEdgeRef(GraphEdge* edge);

    // Delete section
    GraphTerm::List getRootNodes();
    GraphTerm::List getLeafNodes();

    GraphEdge::List getEdgesToLeafs() const;
    GraphEdge::List getEdgesToRoots() const;
    GraphEdge::List getAllConnectedEdges() const;

    int cycleSearchFlag = 0;

    void removeEdgeToLeafs(GraphEdge* edge);
    void removeEdgeToRoots(GraphEdge* edge);

    void            addBrokenEdge(GraphEdge* edge);
    void            addRedundantEdge(GraphEdge* edge);
    GraphEdge::List getBrokenEdges() const;
    GraphEdge::List getRedundantEdges() const;

    void checkForExceedEdges();

    void        giveWeights();
    static void resetMaxWeight();

    int    weight() const;
    void   increaseWeight();
    double getRelativeWeight() const;

    const TermDataCache& cache() const;

private:
    GraphEdge::List edgesToOtherGroups; // Пока никак не используются но будут.
    GraphEdge::List brokenEdges;        // Пока никак не используются но будут.
    GraphEdge::List redundantEdges;

    GraphEdge::List edgesToLeafs;
    GraphEdge::List edgesToRoots;

    int          paintLevel = -1;
    unsigned int treeId     = 0; // 0 = no value

    int        mSelfWeight = 0;
    static int mMaxWeight;

    const TermDataCache mDataCache;

    static bool hasTermInRoots(GraphTerm* term, QList<GraphTerm*>& visitList);

    GraphEdge* findLongPathToNode(GraphTerm* node);

    void fillAllParentsList(GraphTerm* searchNode, GraphTerm::List& lst);
};
