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
#include <QLineF>
#include <QProperty>
#include <QSizeF>

#include <handytypes.h>

#include "source/model/enums.h"
#include "source/model/graphicitem/graphicitem.h"
#include "source/model/term/graphterm.h"

// This class contains all info for paint Term somewhere
// Has functions about paint color, positioning. This kind of stuff
class PaintedTerm : public GraphTerm, public GraphicItem
{
public:
    using Ptr    = std::shared_ptr<PaintedTerm>;
    using OptPtr = std::optional<Ptr>;
    using List   = std::vector<Ptr>;

    PaintedTerm(const TermData& info);
    ~PaintedTerm() override = default;

    // Painting tools
    QLineF getRectLine(Qt::Edge side);

    // NodeRect tools
    QRectF getNodeRect(CoordType inCoordinates) const;
    QRectF getFrameRect(CoordType inCoordinates) const;

    QPointF getCenter(CoordType inCoordinates) const;

    void setSelection(bool selected);
    void setRelativeSelection(bool relativeSelected);
    bool isSelectedAnyway() const;

    QString decoratedTerm() const;
    QRectF  rect() const;
    qreal   cornerRadius() const;

private: // Members
    QProperty<QSizeF> pNodeSize;
    QProperty<qreal>  pCornerRadius;

    bool mThisSelected  = false;
    bool mRelativePaint = false;

    // Color tools
    static QColor baseColor(NodeType type, bool selected);
};
