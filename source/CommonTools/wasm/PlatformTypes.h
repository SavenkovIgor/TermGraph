// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <functional>
#include <utility>

#include "source/CommonTools/HandyTypes.h"

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
using FutureExpected = FutureWrapper<Expected<RetType>>;

template<typename T>
static FutureWrapper<T> toFuture(const std::function<T()>& func)
{
    return FutureWrapper<T>(std::move(func()));
}
