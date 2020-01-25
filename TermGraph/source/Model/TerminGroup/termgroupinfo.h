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

#include <stdlib.h>
#include <functional>
#include <QDebug>
#include <QUuid>

#include "source/Model/TerminGroup/termtree.h"
#include "source/Model/Termin/paintedterm.h"
#include "source/Helpers/tagprocessor.h"
#include "source/Model/TerminGroup/groupinfocontainer.h"

class TermGroupInfo
{
public:
    TermGroupInfo() = delete;
    TermGroupInfo(const GroupInfoContainer& info);
    ~TermGroupInfo();

    // Group info
    QUuid getUuid() const;
    QString getName() const;

    // Group content
    PaintedTerm::List getAllNodes() const;

    // Edges tools
    Edge::List getAllEdges() const;
    Edge::List filterFromEdgesList(std::function<bool(Edge*)> condition) const;
    Edge::List getBrokenEdges() const;
    Edge::List getAllEdgesForPainting() const;

protected:
    // Group content methods
    void addNodeToList(PaintedTerm* node);
    void clearNodesList();

    // Nodes tools
    PaintedTerm::List getRootNodes() const;
    PaintedTerm::List getInTreeNodes() const;
    PaintedTerm::List getOrphanNodes() const;

    // Edges tools
    void loadEdges();
    void removeCycles();
    void removeExceedEdges();

    // Tree tools
    void setLevels();
    void initTrees();
    QSizeF getAllTreesSize();
    void setNeighbours();

    // Orphans tools
    QSizeF getOrphansSize();

    // Termgroup
    TermTree::List trees;

private:
    // Group info
    GroupInfoContainer info;

    // Group content
    PaintedTerm::List nodesList;
    PaintedTerm::List filterFromNodesList(std::function<bool(PaintedTerm*)> filterCheck) const;
            // Filters nodes from nodesList with lambda

    // Edges & tools
    Edge::List edgesList;
    Edge* addNewEdge(PaintedTerm* node1, PaintedTerm* node2);

    // Edge connections search
    Edge::List searchAllConnections();
    QMap<QString, PaintedTerm*> getExactTermMatchCache();
    PaintedTerm* getNearestNodeForTag(const QString& tag);

    Edge::List suggestConnections();  // TODO: Realize!
};
