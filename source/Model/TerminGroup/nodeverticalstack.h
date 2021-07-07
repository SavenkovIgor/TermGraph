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

#include <vector>

#include "source/Model/Termin/paintedterm.h"
#include "source/Model/TerminGroup/paintedforest.h"

class PaintedForest;

class NodeVerticalStackTools
{
public:
    static QSizeF getNodeVerticalStackedSize(const PaintedTerm::List& nodes);
};

class NodeVerticalStack
{
public:
    using List     = std::vector<NodeVerticalStack>;
    using NodePack = QPair<QPointF, PaintedTerm::List>;

    NodeVerticalStack(PaintedForest* parentForest);
    ~NodeVerticalStack() = default;

    void addTerm(PaintedTerm::Ptr term);
    void placeTerms(QPointF centerPoint);

    QSizeF size() const;
    bool   hasNode(PaintedTerm::Ptr term) const;

    PaintedTerm::List nodes() const;

private:
    static std::vector<NodePack> getNodePacks(const PaintedTerm::List& terms, const PaintedForest* forest);
    static void                  sortNodePacks(std::vector<NodePack>& pack);
    static PaintedTerm::List     flatNodePack(const std::vector<NodePack>& pack);

private: // Members
    PaintedTerm::List    mTerms;
    const PaintedForest* mParentForest;
};
