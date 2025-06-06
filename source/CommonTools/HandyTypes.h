// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QUuid>
#include <QtCore>
#include <QtGlobal>

#include "source/CommonTools/Errors.h"

using UuidList = std::vector<QUuid>;

template<typename T>
class Result : public std::variant<T, ErrorCode>
{
public:
    using std::variant<T, ErrorCode>::variant;

    operator bool() const { return !std::holds_alternative<ErrorCode>(*this); }
    bool has_value() const { return !std::holds_alternative<ErrorCode>(*this); }

    T          value() const { return std::get<T>(*this); }
    ErrorCode error() const { return std::get<ErrorCode>(*this); }
};

enum class Direction { Left, Right };

enum class UuidMode { Default, Url };
