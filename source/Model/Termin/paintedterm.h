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

#include <cmath>

#include <QList>
#include <QMap>
#include <QObject>
#include <QSizeF>

#include "source/Helpers/handytypes.h"
#include "source/Model/GraphicItem/graphicitem.h"
#include "source/Model/Termin/graphterm.h"
#include "source/Model/TerminEdge/edge.h"
#include "source/Model/enums.h"

// This class contains all info for paint Term somewhere
// Has functions about paint color, positioning. This kind of stuff
class PaintedTerm : public QObject, public GraphTerm, public GraphicItem
{
    Q_OBJECT

    Q_PROPERTY(QString term READ decoratedTerm CONSTANT)
    Q_PROPERTY(QColor color READ color NOTIFY selectionChanged)
    Q_PROPERTY(qreal radius READ cornerRadius CONSTANT)
    Q_PROPERTY(QRectF rect READ rect CONSTANT)
    Q_PROPERTY(double weight READ getRelativeWeight CONSTANT)

    Q_PROPERTY(bool isSelectedAnyway READ isSelectedAnyway NOTIFY selectionChanged)
    Q_PROPERTY(bool isThisSelected READ isThisSelected NOTIFY selectionChanged)

    QRectF rect() const;

public:
    PaintedTerm(const TermData& info, QObject* parent = nullptr);
    ~PaintedTerm() override = default;

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

    qreal cornerRadius() const;

    void setSelection(const bool& selected);
    bool isSelectedAnyway() const;
    bool isThisSelected() const;

    opt<QPointF> optimalRootsBasedPosition() const;

    QString decoratedTerm() const;

signals:
    void selectionChanged();

protected:
    // Geometry tools
    // --- Methods ---
    void adjustRectSizeForName();

private:
    QSizeF nodeSize = QSizeF(40.0, 10.0);

    bool mThisSelected  = false;
    bool mRelativePaint = false;

    // Rect tools
    static QRectF addMarginsToRect(QRectF rc, qreal mrg);

    // Color tools
    QColor        color() const;
    static QColor baseColor(NodeType type, bool selected);

    qreal mCornerRadius = 0;
    void  updateCornerRadius();
};
