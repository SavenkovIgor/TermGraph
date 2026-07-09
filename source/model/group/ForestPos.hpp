// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QMetaType>

struct ForestPos {
    Q_GADGET
    Q_PROPERTY(size_t depth    MEMBER depth    FINAL)
    Q_PROPERTY(size_t stackPos MEMBER stackPos FINAL)

    size_t depth    = 0;
    size_t stackPos = 0;
};

Q_DECLARE_METATYPE(ForestPos)
