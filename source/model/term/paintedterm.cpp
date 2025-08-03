// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <vector>

#include <QLineF>
#include <QProperty>
#include <QSizeF>

#include "source/TermDataInterface/TermData.h"

export module PaintedTerm;

import Enums.CoordType;
import GraphicItem;
import GraphTerm;

using namespace std;

// This class contains all info for paint Term somewhere
// Has functions about paint color, positioning. This kind of stuff
export class PaintedTerm : public GraphTerm, public GraphicItem
{
public:
    using Ptr    = shared_ptr<PaintedTerm>;
    using OptPtr = optional<Ptr>;
    using List   = vector<Ptr>;

    static inline auto asListSize = [](auto num) { return static_cast<List::size_type>(num); };

    PaintedTerm(const TermData& info)
        : GraphTerm(info)
    {
        pCornerRadius.setBinding([this]() { return min(pNodeSize.value().height() * 0.15, 12.0); });

        pNodeSize.setBinding([this]() -> QSizeF {
            auto prefSize = cache().preferredSize.value();
            auto offsets  = cache().definition().isEmpty() ? QSizeF(34, 16) : QSizeF(34, 34);
            return prefSize + offsets;
        });
    }

    ~PaintedTerm() override = default;

    // NodeRect tools
    QRectF getNodeRect(CoordType inCoordinates) const
    {
        switch (inCoordinates) {
        case CoordType::zeroPoint: return QRectF(QPointF(), pNodeSize.value());
        case CoordType::local: return QRectF(pos(), pNodeSize.value());
        case CoordType::scene: return QRectF(scenePos(), pNodeSize.value());
        }

        assert(false); // must be unreachable
        return {};
    }

    QRectF getFrameRect(CoordType inCoordinates) const
    {
        qreal  val = qBound(0.0, pNodeSize.value().width() * 0.2, 8.0);
        QRectF ret = getNodeRect(inCoordinates);
        return ret.marginsAdded({val, val, val, val});
    }

    QPointF getCenter(CoordType inCoordinates) const { return getNodeRect(inCoordinates).center(); }

    void setSelection(bool selected)
    {
        if (mThisSelected != selected) {
            mThisSelected  = selected;
            mRelativePaint = selected;
        }
    }

    void setRelativeSelection(bool relativeSelected) { mRelativePaint = relativeSelected; }
    bool isSelectedAnyway() const { return mThisSelected || mRelativePaint; }

    QString term() const { return cache().term(); }
    QString definition() const { return cache().definition(); }
    QRectF  rect() const { return getNodeRect(CoordType::scene); }
    qreal   cornerRadius() const { return pCornerRadius.value(); }

    bool isGhost() const { return mIsGhost; }
    void setGhost(bool ghost) { mIsGhost = ghost; }

private: // Members
    QProperty<QSizeF> pNodeSize;
    QProperty<qreal>  pCornerRadius;

    bool mThisSelected  = false;
    bool mRelativePaint = false;

    bool mIsGhost = false;
};
