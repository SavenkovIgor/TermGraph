// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/model/term/paintedterm.h"

PaintedTerm::PaintedTerm(const TermData& info)
    : GraphTerm(info)
{
    pCornerRadius.setBinding([this]() { return std::min(pNodeSize.value().height() * 0.15, 12.0); });

    pNodeSize.setBinding([this]() -> QSizeF {
        auto prefSize = cache().preferredSize.value();
        auto offsets  = cache().definition().isEmpty() ? QSizeF(34, 16) : QSizeF(34, 34);
        return prefSize + offsets;
    });
}

QRectF PaintedTerm::getNodeRect(CoordType inCoordinates) const
{
    switch (inCoordinates) {
    case CoordType::zeroPoint: return QRectF(QPointF(), pNodeSize.value());
    case CoordType::local: return QRectF(pos(), pNodeSize.value());
    case CoordType::scene: return QRectF(scenePos(), pNodeSize.value());
    }

    assert(false); // must be unreachable
    return {};
}

QRectF PaintedTerm::getFrameRect(CoordType inCoordinates) const
{
    qreal  val = qBound(0.0, pNodeSize.value().width() * 0.2, 8.0);
    QRectF ret = getNodeRect(inCoordinates);
    return ret.marginsAdded({val, val, val, val});
}

QPointF PaintedTerm::getCenter(CoordType inCoordinates) const { return getNodeRect(inCoordinates).center(); }

qreal PaintedTerm::cornerRadius() const { return pCornerRadius.value(); }

bool PaintedTerm::isGhost() const { return mIsGhost; }

void PaintedTerm::setGhost(bool ghost) { mIsGhost = ghost; }

void PaintedTerm::setSelection(bool selected)
{
    if (mThisSelected != selected) {
        mThisSelected  = selected;
        mRelativePaint = selected;
    }
}

void PaintedTerm::setRelativeSelection(bool relativeSelected) { mRelativePaint = relativeSelected; }

bool PaintedTerm::isSelectedAnyway() const { return mThisSelected || mRelativePaint; }

QString PaintedTerm::term() const { return cache().term(); }

QString PaintedTerm::definition() const { return cache().definition(); }

QRectF PaintedTerm::rect() const { return getNodeRect(CoordType::scene); }
