// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <functional>

#include <QFuture>
#include <QPromise>

#include "source/CommonTools/HandyTypes.h"

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
