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

#include <optional>
#include <vector>

#include <outcome.hpp>

#include <QFuture>
#include <QSizeF>
#include <QUuid>
#include <QtCore>

namespace outcome = OUTCOME_V2_NAMESPACE;

using UuidList   = std::vector<QUuid>;

template<typename T>
using Opt = std::optional<T>;

template<typename T>
using Result = outcome::std_result<T>;

template<typename RetType>
//using FutureRes = QFuture<Result<RetType>>;
// This wrapper is need for quick and transparent replace of QFuture<Result> with just Result
class FutureRes : public QFuture<Result<RetType>>
{
public:
    FutureRes(const QFuture<Result<RetType>>& base)
        : QFuture<Result<RetType>>(std::move(base))
    {}

    RetType value() { return QFuture<Result<RetType>>::result().value(); }

    operator bool() { return static_cast<bool>(QFuture<Result<RetType>>::result()); }

    bool has_value() { return QFuture<Result<RetType>>::result().has_value(); }

    bool has_error() { return QFuture<Result<RetType>>::result().has_error(); }

    auto error() { return QFuture<Result<RetType>>::result().error(); }
};

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
