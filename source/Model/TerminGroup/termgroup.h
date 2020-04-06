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

#include <QObject>

#include "source/Helpers/handytypes.h"
#include "source/Model/GraphicItem/rectgraphicitem.h"
#include "source/Model/TerminGroup/termgroupinfo.h"

class TermGroup : public QObject, public TermGroupInfo
{
    Q_OBJECT

public:
    TermGroup(const GroupInfoContainer& info, QObject* parent = nullptr);
    ~TermGroup();

    void initNewNodes();

    RectGraphicItem* baseRect;

    QRectF  getGroupRect() const;

    UuidList searchNearest(const QString& text, int limit = 10);
    UuidList searchContains(const QString& text, int limit = 10);

    // Position and frame
    void setBasePoint(QPointF pt);
    void updateGroupFrame();

    void loadNodes(const PaintedTerm::List& newNodes);

    PaintedTerm* getNode(const QPointF& pt) const;
    PaintedTerm* getNode(const QUuid& nodeUuid) const;
    PaintedTerm* getNode(const QString& nodeName) const;

public slots:
    void sceneUpdateSignal();

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
    void              addTreeRectsToScene();
    RectGraphicItem*  orphansRect;

    QSizeF getNameSize() const;
};
