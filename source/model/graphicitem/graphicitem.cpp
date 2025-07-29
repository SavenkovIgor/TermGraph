// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QPointF>
#include <QRectF>

export module GraphicItem;

export class GraphicItem
{
public:
    GraphicItem()          = default;
    virtual ~GraphicItem() = default;

    // Parents
    void setParentItem(GraphicItem* parent) { mParent = parent; }

    // Position
    void    setPos(const QPointF& pt) { mPosition = pt; }
    QPointF pos() const { return mPosition; }
    QPointF scenePos() const
    {
        if (mParent == nullptr) {
            return mPosition;
        }

        return mPosition + mParent->scenePos();
    }

private:
    QPointF      mPosition;
    GraphicItem* mParent = nullptr;
};
