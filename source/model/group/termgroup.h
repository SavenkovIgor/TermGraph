// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "source/TermDataInterface/GroupSummary.h"

#include "source/helpers/linktermdistancecache.h"
#include "source/model/graphicitem/rectgraphicitem.h"
#include "source/model/group/paintedforest.h"
#include "source/model/term/paintedterm.h"

class TermGroup
{
    using GraphT = graph::Graph<PaintedTerm, PaintedEdge>;

public:
    using Ptr    = std::shared_ptr<TermGroup>;
    using OptPtr = std::optional<Ptr>;

    TermGroup(const GroupSummary& info, const TermData::List& termData);
    ~TermGroup() = default;

    void setBasePoint(QPointF pt);

    QRectF getGroupRect() const;

    std::vector<QUuid> searchNearest(const QString& text, int limit = 10) const;
    std::vector<QUuid> searchContains(const QString& text, int limit = 10) const;

    PaintedTerm::OptPtr getTerm(const QPointF& pt) const;
    PaintedTerm::OptPtr getTerm(const QUuid& termUuid) const;
    PaintedTerm::OptPtr getTerm(const QString& termName) const;

    PaintedTerm::List terms() const;
    PaintedEdge::List edgesForPaint() const;

    QUuid   uuid() const;
    QString name() const;

    QString getHierarchyDefinition(PaintedTerm::Ptr term);

    void selectTerm(const PaintedTerm::Ptr& term, bool selection);

    NodeType::Type termType(const PaintedTerm::Ptr& term) const;

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

    PaintedTerm::List collapseSynonyms(PaintedTerm::List terms);

    // Main objects
    void addTreeRectsToScene();

    QSizeF getNameSize() const;

    QString qmlUuid() const;

    // External
    QSizeF getAllTreesSize();

    struct SearchConnectionResult
    {
        PaintedTerm::List ghostTerms;
        PaintedEdge::List edges;
    };

    // Edges
    SearchConnectionResult searchAllConnections(const PaintedTerm::List& terms);

    std::optional<PaintedTerm::Ptr> findLinkTarget(const QString& link, const PaintedTerm::List& terms);
    PaintedEdge::List               filterFromEdgesList(std::function<bool(PaintedEdge::Ptr)> condition) const;

    // Nodes
    PaintedTerm::List               getRootNodes() const;
    QMap<QString, PaintedTerm::Ptr> createExactLinkMatchCacheFor(const PaintedTerm::List& terms);
    QMap<QUuid, PaintedTerm::Ptr>   createUuidCacheFor(const PaintedTerm::List& terms);
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

    GroupSummary mInfo;

    PaintedForest::List mForests;

    GraphT mGraphData = GraphT({});

    static LinkTermDistanceCache linkCache;
};
