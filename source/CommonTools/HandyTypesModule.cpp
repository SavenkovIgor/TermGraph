// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <vector>

#include <QSizeF>

export module CommonTools.HandyTypes;

export auto asInt(auto num) -> int { return static_cast<int>(num); }

export class SizeList : public std::vector<QSizeF>
{
public:
    // Returns total size of stacked sizes, placed vertically or horizontally
    QSizeF totalStackedSize(Qt::Orientation stackDirection)
    {
        qreal width  = 0;
        qreal height = 0;

        if (stackDirection == Qt::Vertical) {
            for (auto& size : (*this)) {
                width = std::max(width, size.width());
                height += size.height();
            }
        } else if (stackDirection == Qt::Horizontal) {
            for (auto& size : (*this)) {
                width += size.width();
                height = std::max(height, size.height());
            }
        }

        return {width, height};
    }
};
