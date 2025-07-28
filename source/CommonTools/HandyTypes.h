// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <expected>

#include <QUuid>
#include <QtCore>
#include <QtGlobal>

#include "source/CommonTools/Errors.h"

template<typename T>
using Expected = std::expected<T, ErrorCode>;
