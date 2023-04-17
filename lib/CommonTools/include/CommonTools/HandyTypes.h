// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <vector>

#include <outcome.hpp>

#include <QFuture>
#include <QSizeF>
#include <QUuid>
#include <QtCore>

#include <CommonTools/Errors.h>

namespace outcome = OUTCOME_V2_NAMESPACE;

using UuidList   = std::vector<QUuid>;

template<typename T>
using Opt = std::optional<T>;

template<typename T>
using Result = outcome::std_result<T>;

template<typename RetType>
using FutureResult = QFuture<Result<RetType>>;

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

inline auto asInt(auto num) -> int { return static_cast<int>(num); }

template<typename T>
static QFuture<T> toFuture(const std::function<T()>& func)
{
    QPromise<T> promise;
    promise.start();
    promise.addResult(func());
    promise.finish();
    return promise.future();
}
