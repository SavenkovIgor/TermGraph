// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <vector>
#include <future>

#include <outcome.hpp>

#include <QSizeF>
#include <QUuid>
#include <QtCore>

namespace outcome = OUTCOME_V2_NAMESPACE;

using UuidList   = std::vector<QUuid>;

template<typename T>
using Opt = std::optional<T>;

template<typename T>
using Result = outcome::std_result<T>;

template<typename T>
using FutureResult = std::future<Result<T>>;

enum class Direction { Left, Right };

enum class UuidMode { Default, Url };

class SizeList : public std::vector<QSizeF>
{
public:
    // Retruns total size of stacked sizes, placed vertically or horizontally
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
