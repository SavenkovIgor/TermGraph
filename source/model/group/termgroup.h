// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

#include <CommonTools/HandyTypes.h>
#include <TermDataInterface/GroupSummary.h>

#include "source/model/graphicitem/rectgraphicitem.h"
#include "source/model/group/paintedforest.h"
#include "source/model/term/paintedterm.h"
#include "source/helpers/linktermdistancecache.h"


class TermGroup : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString uuid READ qmlUuid CONSTANT FINAL)
    Q_PROPERTY(QString name READ name CONSTANT FINAL)

    using GraphT = graph::Graph<PaintedTerm, PaintedEdge>;

public:
    using Ptr    = std::shared_ptr<TermGroup>;
    using OptPtr = std::optional<Ptr>;

    TermGroup(const GroupSummary& info, const TermData::List& termData, QObject* parent = nullptr);
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

    // Main objects
    void addTreeRectsToScene();

    QSizeF getNameSize() const;

    QString qmlUuid() const;

    // External
    QSizeF getAllTreesSize();

    // Edges
    PaintedEdge::List searchAllConnections(const PaintedTerm::List& terms);
    PaintedEdge::List suggestConnections(); // TODO: Realize!

    Opt<PaintedTerm::Ptr> getNearestNodeForTag(const QString& tag, const PaintedTerm::List& terms);
    PaintedEdge::List     filterFromEdgesList(std::function<bool(PaintedEdge::Ptr)> condition) const;

    // Nodes
    PaintedTerm::List               getRootNodes() const;
    QMap<QString, PaintedTerm::Ptr> createExactLinkMatchCacheFor(const PaintedTerm::List& terms);
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

    GroupSummary mInfo;

    PaintedForest::List mForests;

    GraphT mGraphData = GraphT({});

    static LinkTermDistanceCache linkCache;
};
