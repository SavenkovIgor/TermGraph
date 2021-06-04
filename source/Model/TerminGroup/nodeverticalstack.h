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

#include <QList>

#include "source/Model/Termin/paintedterm.h"
#include "source/Model/TerminEdge/paintededge.h"

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

    NodeVerticalStack()  = default;
    ~NodeVerticalStack() = default;

    void addTerm(PaintedTerm* term);
    void placeTerms(QPointF centerPoint);

    QSizeF size() const;
    bool   isRootStack() const;

    bool hasNode(PaintedTerm* term) const;

    PaintedTerm::List nodes() const;

private:
    static QList<NodePack>   getNodePacks(const PaintedTerm::List& terms);
    static void              sortNodePacks(QList<NodePack>& pack);
    static PaintedTerm::List flatNodePack(const QList<NodePack>& pack);

private:  // Members
    PaintedTerm::List mTerms;
};
