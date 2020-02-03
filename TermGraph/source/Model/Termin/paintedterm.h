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

#include <cmath>

#include <QList>
#include <QMap>
#include <QObject>
#include <QSizeF>
#include <QSqlRecord>

#include "source/Model/GraphicItem/graphicitem.h"
#include "source/Model/Termin/graphterm.h"
#include "source/Model/TerminEdge/edge.h"
#include "source/Model/enums.h"

// This class contains all info for paint Term somewhere
// Has functions about paint color, positioning. This kind of stuff
class PaintedTerm : public QObject, public GraphTerm, public GraphicItem
{
    Q_OBJECT

    Q_PROPERTY(QString term READ getSmallName CONSTANT)
    Q_PROPERTY(QColor color READ color NOTIFY colorChanged)
    Q_PROPERTY(qreal radius READ getCornerRadius CONSTANT)
    Q_PROPERTY(QRectF rect READ rect CONSTANT)

    QRectF rect() const;

public:
    PaintedTerm(const NodeInfoContainer& info, QObject* parent = nullptr);
    ~PaintedTerm() override = default;

    bool thisSelected = false;

    using List = QList<PaintedTerm*>;
    using Map  = QMap<QString, PaintedTerm*>;

    static bool someoneHover;
    static bool someoneSelect;

    static QList<Qt::Edge> sides;

    // Leveling tools
    int getUpLevels(int pLevel = -1);

    // Painting tools
    void   setRelatedPaintUp(bool val);
    void   setRelatedPaintDown(bool val);
    QLineF getRectLine(Qt::Edge side);

    // NodeRect tools
    QRectF getNodeRect(CoordType inCoordinates) const;
    QRectF getFrameRect(CoordType inCoordinates) const;

    QPointF getCenter(CoordType inCoordinates) const;

    // Animation
    bool applyMove();
    void countForces();

    // Edges work
    int   getIntersections(bool swapped = false);
    qreal getSumEdgesLength(bool swap);

    void setSwap(QPointF toPt);
    void dropSwap();

    qreal getCornerRadius() const;

    void setSelection(const bool& selected);

    // Color tools
    void initColor();
    void checkColor();

signals:
    void colorChanged();

protected:
    // Geometry tools
    // --- Methods ---
    void adjustRectSizeForName();

    // --- Variables ---
    // Hovers
    bool relativePaint = false;

private:
    QSizeF nodeSize = QSizeF(40.0, 10.0);
    QColor mColor;

    // Scene tools
    static bool isNearPoints(QPointF pt1, QPointF pt2, qreal dist);

    // Neighbours tools
    PaintedTerm* getNearestLeftNeigh();
    PaintedTerm* getNearestRightNeigh();

    // Edges tools
    Edge::List getEdgesInLayer();

    // Rect tools
    static QRectF addMarginsToRect(QRectF rc, qreal mrg);

    // --- Variables ---
    static const qreal verScale;

    // Paint / Animation
    qreal newPosOffset = 0.0;

    // Color tools
    QColor        color() const;
    QColor        expectedColor() const;
    static QColor baseColor(NodeType type, bool selected);

    qreal cornerRadius = 0;
    void  updateCornerRadius();
};
