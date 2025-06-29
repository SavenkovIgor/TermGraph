// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <functional>

#define EXCLUSIVE_CALL(f) static auto beforeMain = ExclusiveCall([]() f);

class ExclusiveCall
{
public:
    ExclusiveCall(std::function<void()> func)
    {
        func();
        exit(0);
    }
};
