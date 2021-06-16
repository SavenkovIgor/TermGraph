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

#include <QColor>
#include <QProperty>
#include <QSizeF>

#include "source/Helpers/handytypes.h"
#include "source/Model/GraphicItem/graphicitem.h"
#include "source/Model/Termin/graphterm.h"
#include "source/Model/enums.h"

// This class contains all info for paint Term somewhere
// Has functions about paint color, positioning. This kind of stuff
class PaintedTerm : public GraphTerm, public GraphicItem
{
public:
    PaintedTerm(const TermData& info);
    ~PaintedTerm() override = default;

    using UnsafeList = std::vector<PaintedTerm*>;

    // Painting tools
    QLineF getRectLine(Qt::Edge side);

    // NodeRect tools
    QRectF getNodeRect(CoordType inCoordinates) const;
    QRectF getFrameRect(CoordType inCoordinates) const;

    QPointF getCenter(CoordType inCoordinates) const;

    void setSelection(const bool& selected);
    bool isSelectedAnyway() const;

    opt<QPointF> optimalRootsBasedPosition() const;

    QString decoratedTerm() const;
    QRectF  rect() const;
    qreal   cornerRadius() const;
    QColor  color() const;

private: // Methods
    void setRelatedPaintUp(bool val);
    void setRelatedPaintDown(bool val);

private: // Members
    QProperty<QSizeF> pNodeSize;
    QProperty<qreal>  pCornerRadius;

    bool mThisSelected  = false;
    bool mRelativePaint = false;

    // Color tools
    static QColor baseColor(NodeType type, bool selected);
};
