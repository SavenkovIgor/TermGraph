// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <vector>

#include <QLineF>
#include <QProperty>
#include <QSizeF>

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

    static inline auto asListSize = [](auto num) { return static_cast<List::size_type>(num); };

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

    QString term() const;
    QString definition() const;
    QRectF  rect() const;
    qreal   cornerRadius() const;

    bool isGhost() const;
    void setGhost(bool ghost);

private: // Members
    QProperty<QSizeF> pNodeSize;
    QProperty<qreal>  pCornerRadius;

    bool mThisSelected  = false;
    bool mRelativePaint = false;

    bool mIsGhost = false;
};
