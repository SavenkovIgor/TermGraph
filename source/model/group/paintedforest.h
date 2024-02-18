// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <vector>

#include <Graph/Forest.hpp>

#include "source/model/edge/paintededge.h"
#include "source/model/graphicitem/rectgraphicitem.h"
#include "source/model/group/nodeverticalstack.h"
#include "source/model/term/paintedterm.h"

class NodeVerticalStack;

class PaintedForest : public graph::Forest<PaintedTerm, PaintedEdge>
{
public:
    using Ptr  = std::shared_ptr<PaintedForest>;
    using List = std::vector<Ptr>;

    // Constructor/Destructor
    PaintedForest(const GraphData<PaintedTerm, PaintedEdge>& data);
    ~PaintedForest() = default;

    RectGraphicItem& rect();

    // TreeInfo
    bool hasTerm(PaintedTerm::Ptr term) const;

    QRectF frameRect(CoordType inCoordinates) const;
    QSizeF baseSize() const; // Clearly counted value. Ignoring real node positions
    double square() const;

    // Internal counts and preparations
    void setTreeNodeCoords(QPointF leftTopPoint = QPointF());

    Opt<QPointF> optimalRootsBasedPosition(const PaintedTerm::Ptr term) const;

    PaintedTerm::OptPtr getNodeAtPoint(const QPointF& pt) const;

    QString getHierarchyDefinition(PaintedTerm::Ptr term);

    void selectTerm(const PaintedTerm::Ptr& term, bool selected);

    // Deletions
    PaintedForest(const PaintedForest&) = delete;

private:
    // Internal Info
    PaintedTerm::List getAllNodesInTree() const;
    qreal             getMaxStackHeight() const;

private: // Members
    std::vector<NodeVerticalStack> mStacks;
    RectGraphicItem                mRect;
};
