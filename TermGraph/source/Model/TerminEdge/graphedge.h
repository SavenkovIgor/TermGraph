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

class GraphTerm;

class GraphEdge
{
public:
    using List = QList<GraphEdge*>;

    GraphEdge(GraphTerm* toRoot, GraphTerm* toLeaf);
    virtual ~GraphEdge() = default;

    GraphTerm* getRoot() const { return toRoot; }
    GraphTerm* getLeaf() const { return toLeaf; }

    bool hasNode(GraphTerm* node) const;
    bool isSameEdge(GraphTerm* rt, GraphTerm* br) const;
    GraphTerm* getOtherSide(GraphTerm* n) const;

    bool needPaint = true;  // Flag for placing in paintQueue
    bool needBroke = false;
    bool needCutOut = false;

private:
    GraphTerm* toRoot;
    GraphTerm* toLeaf;
};
