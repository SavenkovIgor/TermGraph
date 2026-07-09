// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QMetaType>

struct OrphanPos {
    Q_GADGET
    Q_PROPERTY(size_t index MEMBER index FINAL)

    size_t index = 0;
};

Q_DECLARE_METATYPE(OrphanPos)
