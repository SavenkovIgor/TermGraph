// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QPointF>
#include <QRectF>

class GraphicItem
{
public:
    GraphicItem()          = default;
    virtual ~GraphicItem() = default;

    // Parents
    void setParentItem(GraphicItem* parent);

    // Position
    void    setPos(const QPointF& pt);
    QPointF pos() const;
    QPointF scenePos() const;

private:
    QPointF      mPosition;
    GraphicItem* mParent = nullptr;
};
