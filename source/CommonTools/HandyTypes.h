// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <vector>

#include <QSizeF>
#include <QUuid>
#include <QtCore>
#include <QtGlobal>

#include "source/CommonTools/Errors.h"

using UuidList = std::vector<QUuid>;

template<typename T>
class Result : public std::variant<T, ErrorCodes>
{
public:
    using std::variant<T, ErrorCodes>::variant;

    operator bool() const { return !std::holds_alternative<ErrorCodes>(*this); }
    bool has_value() const { return !std::holds_alternative<ErrorCodes>(*this); }

    T          value() const { return std::get<T>(*this); }
    ErrorCodes error() const { return std::get<ErrorCodes>(*this); }
};

enum class Direction { Left, Right };

enum class UuidMode { Default, Url };

class SizeList : public std::vector<QSizeF>
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
