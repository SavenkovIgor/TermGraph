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
#include <QTimer>

#include "source/Helpers/appstyle.h"
#include "source/Helpers/tagprocessor.h"
#include "source/Model/GraphicItem/rectgraphicitem.h"
#include "source/Model/TerminGroup/termgroupinfo.h"
#include "source/Model/TerminEdge/edge.h"
#include "source/Model/GraphicItem/labelgraphicitem.h"

class TermGroup : public QObject, public TermGroupInfo
{
    Q_OBJECT

    static int animSpeed;

    QTimer animTimer;
    QTimer checkSwapTimer;

public:
    TermGroup(const GroupInfoContainer& info, QObject* parent = nullptr);
    ~TermGroup();

    void initNewNodes();

    RectGraphicItem *baseRect;

    QString getNameLabel();
    QPointF getNamePos();
    QRectF getGroupRect() const;
    QColor getGroupColor();
    QColor getGroupFillColor();

    void setAnimSpeed(int val);
    static int getAnimSpeed();

    UuidList searchNearest(const QString& text, int limit = 10);
    UuidList searchContains(const QString& text, int limit = 10);

    // Position and frame
    void setBasePoint(QPointF pt);
    void updateGroupFrame();

    void loadNodes(PaintedTerm::List newNodes);

    bool needPaint() const;
    void resetPaintFlags();

    PaintedTerm* getNode(const QPointF &pt) const;
    PaintedTerm* getNode(const QUuid nodeUuid) const;

    void checkColors(bool init = false);

public slots:
    void checkSwap();

    void startAnimation();
    void stopAnimation();

    void sceneUpdateSignal();

private slots:
    void animateGroup();

private:
    // Base init
    void addOrphansToParents();
    void addEdgesToParents();

    // Group
    qreal getGroupMinWidth();
    void updateRectsPositions();
    void updateBaseRectSize();

    // Tree
    void setTreeCoords();

    // Orphans
    void setOrphCoords(qreal maxWidth = 200.0);

    // Main objects
    LabelGraphicItem* grNmItem;
    void addTreeRectsToScene();
    RectGraphicItem *orphansRect;
};
