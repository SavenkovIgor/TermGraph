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
#include "source/Model/GraphicItem/rectgraphicitem.h"
#include "source/Model/Termin/paintedterm.h"
#include "source/Model/TerminEdge/paintededge.h"
#include "source/Model/TerminGroup/groupdata.h"
#include "source/Model/TerminGroup/termtree.h"

class TermGroup : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString uuid READ qmlUuid CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)

public:
    TermGroup(const GroupData& info, const PaintedTerm::List& newNodes, QObject* parent = nullptr);
    ~TermGroup();

    void setBasePoint(QPointF pt);

    QRectF getGroupRect() const;

    UuidList searchNearest(const QString& text, int limit = 10) const;
    UuidList searchContains(const QString& text, int limit = 10) const;

    PaintedTerm* getNode(const QPointF& pt) const;
    PaintedTerm* getNode(const QUuid& nodeUuid) const;
    PaintedTerm* getNode(const QString& nodeName) const;

    PaintedTerm::List nodes() const;
    PaintedEdge::List edgesForPaint() const;

    QUuid   uuid() const;
    QString name() const;

private:
    // Base init
    void addOrphansToParents();
    void addEdgesToParents();

    // Group
    qreal getGroupMinWidth();
    void  updateRectsPositions();
    void  updateBaseRectSize();

    // Tree
    void setTreeCoords();

    // Orphans
    void setOrphCoords(qreal maxWidth = 200.0);

    // Weights
    void setAllWeights();

    // Main objects
    void addTreeRectsToScene();

    QSizeF getNameSize() const;

    QString qmlUuid() const;

    // External
    // Trees
    void           initTrees();
    TermTree::List trees() const;
    QSizeF         getAllTreesSize();

    // Edges
    PaintedEdge::List searchAllConnections();
    PaintedEdge::List suggestConnections(); // TODO: Realize!

    PaintedTerm*      getNearestNodeForTag(const QString& tag);
    void              removeExceedEdges();
    PaintedEdge::List brokenEdges() const;
    PaintedEdge::List redundantEdges() const;
    PaintedEdge::List filterFromEdgesList(std::function<bool(PaintedEdge*)> condition) const;
    void              removeCycles();
    PaintedEdge::List edges() const;

    // Nodes
    void                        setLevels();
    PaintedTerm::List           getRootNodes() const;
    QMap<QString, PaintedTerm*> getExactTermMatchCache();
    QMap<QUuid, PaintedTerm*>   getTermUuidsMap();
    QSizeF                      getOrphansSize();
    PaintedTerm::List           getInTreeNodes() const;
    PaintedTerm::List           getOrphanNodes() const;
    PaintedTerm::List           filterFromNodesList(std::function<bool(PaintedTerm*)> filterCheck) const;

    bool isThreadInterrupted();

private:
    qint64 groupCreationTime = 0;

    RectGraphicItem mBaseRect    = RectGraphicItem(QPoint(0, 0), QSizeF(10.0, 10.0));
    RectGraphicItem mOrphansRect = RectGraphicItem();

    Forest<int, int>::List mForests;
    Node<int>::List        mOrphans;

    GroupData mInfo;

    PaintedTerm::List mNodes;
    PaintedEdge::List mEdges;

    TermTree::List mTrees;
};
