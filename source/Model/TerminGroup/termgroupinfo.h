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

#include <functional>
#include <stdlib.h>
#include <QUuid>

#include "source/Model/Termin/paintedterm.h"
#include "source/Model/TerminGroup/groupdata.h"
#include "source/Model/TerminGroup/termtree.h"

class TermGroupInfo
{
public:
    TermGroupInfo(const GroupData& info);
    ~TermGroupInfo();

    // Group info
    QUuid   uuid() const;
    QString name() const;

    // Group content
    PaintedTerm::List nodes() const;

    // Edges tools
    EdgeOld::List edges() const;
    EdgeOld::List brokenEdges() const;
    EdgeOld::List redundantEdges() const;
    EdgeOld::List edgesForPaint() const;

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
    void   setLevels();
    void   initTrees();
    QSizeF getAllTreesSize();

    // Orphans tools
    QSizeF getOrphansSize();

    TermTree::List trees() const;

    bool buildingWasInterrupted();

private:
    // Group content
    PaintedTerm::List filterFromNodesList(std::function<bool(PaintedTerm*)> filterCheck) const;
    EdgeOld::List     filterFromEdgesList(std::function<bool(EdgeOld*)> condition) const;

    // Edge connections search
    EdgeOld::List               searchAllConnections();
    QMap<QString, PaintedTerm*> getExactTermMatchCache();
    QMap<QUuid, PaintedTerm*>   getTermUuidsMap();
    PaintedTerm*                getNearestNodeForTag(const QString& tag);

    EdgeOld::List suggestConnections(); // TODO: Realize!

    void removeTrees();

private: // Members
    // Group info
    GroupData mInfo;

    PaintedTerm::List mNodes;
    EdgeOld::List     mEdges;

    TermTree::List mTrees;
};
