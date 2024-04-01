// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <vector>

#ifndef Q_OS_WASM
#include <QFuture>
#endif

#include <QSizeF>
#include <QUuid>
#include <QtCore>

#include "source/commonTools/Errors.h"

using UuidList = std::vector<QUuid>;

template<typename T>
using Opt = std::optional<T>;

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

#ifdef Q_OS_WASM
// Wrapper, that supports "then" call
template<typename T>
class FutureWrapper
{
public:
    FutureWrapper() = default;

    FutureWrapper(T&& value)
        : mValue(std::move(value))
    {}

    auto then(const std::function<void(T)>& func) -> void { func(std::move(mValue)); }

private:
    T mValue;
};

template<typename RetType>
using FutureResult = FutureWrapper<Result<RetType>>;

template<typename T>
static FutureWrapper<T> toFuture(const std::function<T()>& func)
{
    return FutureWrapper<T>(std::move(func()));
}

#else
template<typename RetType>
using FutureResult = QFuture<Result<RetType>>;

template<typename T>
static QFuture<T> toFuture(const std::function<T()>& func)
{
    QPromise<T> promise;
    promise.start();
    promise.addResult(func());
    promise.finish();
    return promise.future();
}
#endif

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
