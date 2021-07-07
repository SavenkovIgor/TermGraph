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

#include <QObject>

#include "source/Helpers/handytypes.h"
#include "source/Model/Base/forest.hpp"
#include "source/Model/Base/graphdata.hpp"
#include "source/Model/GraphicItem/rectgraphicitem.h"
#include "source/Model/Termin/paintedterm.h"
#include "source/Model/TerminGroup/groupdata.h"
#include "source/Model/TerminGroup/termtree.h"

class TermGroup : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString uuid READ qmlUuid CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)

    using GraphT = Graph<PaintedTerm, PaintedEdge>;

public:
    using Ptr    = std::shared_ptr<TermGroup>;
    using OptPtr = std::optional<Ptr>;

    TermGroup(const GroupData& info, const TermData::List& termData, QObject* parent = nullptr);
    ~TermGroup() = default;

    void setBasePoint(QPointF pt);

    QRectF getGroupRect() const;

    UuidList searchNearest(const QString& text, int limit = 10) const;
    UuidList searchContains(const QString& text, int limit = 10) const;

    PaintedTerm::OptPtr getTerm(const QPointF& pt) const;
    PaintedTerm::OptPtr getTerm(const QUuid& termUuid) const;
    PaintedTerm::OptPtr getTerm(const QString& termName) const;

    PaintedTerm::List terms() const;
    PaintedEdge::List edgesForPaint() const;

    QUuid   uuid() const;
    QString name() const;

    QString getHierarchyDefinition(PaintedTerm::Ptr term);

    void selectTerm(const PaintedTerm::Ptr& term, bool selection);

    NodeType termType(const PaintedTerm::Ptr& term) const;

private:
    // Base init
    void addOrphansToParents();

    // Group
    qreal getGroupMinWidth();
    void  updateRectsPositions();
    void  updateBaseRectSize();

    // Tree
    void setTreeCoords();

    // Orphans
    void setOrphCoords(qreal maxWidth = 200.0);

    // Main objects
    void addTreeRectsToScene();

    QSizeF getNameSize() const;

    QString qmlUuid() const;

    // External
    QSizeF getAllTreesSize();

    // Edges
    PaintedEdge::List searchAllConnections(const PaintedTerm::List& terms);
    PaintedEdge::List suggestConnections(); // TODO: Realize!

    opt<PaintedTerm::Ptr> getNearestNodeForTag(const QString& tag, const PaintedTerm::List& terms);
    PaintedEdge::List     filterFromEdgesList(std::function<bool(PaintedEdge::Ptr)> condition) const;

    // Nodes
    PaintedTerm::List               getRootNodes() const;
    QMap<QString, PaintedTerm::Ptr> getExactTermMatchCache();
    QMap<QUuid, PaintedTerm::Ptr>   getTermUuidsMap();
    QSizeF                          getOrphansSize();
    PaintedTerm::List               getInTreeNodes() const;
    PaintedTerm::List               getOrphanNodes() const;
    PaintedTerm::List               filterFromNodesList(std::function<bool(PaintedTerm::Ptr)> filterCheck) const;

    PaintedEdge::List allEdges() const;
    PaintedEdge::List allBrokenEdges() const;
    PaintedEdge::List allExceedEdges() const;

    bool isThreadInterrupted();

private:
    qint64 groupCreationTime = 0;

    RectGraphicItem mBaseRect    = RectGraphicItem(QPoint(0, 0), QSizeF(10.0, 10.0));
    RectGraphicItem mOrphansRect = RectGraphicItem();

    GroupData mInfo;

    PaintedForest::List mTrees; // TODO: mForests;

    GraphT mGraphData = GraphT({});
};
