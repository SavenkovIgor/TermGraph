// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>

#include <QPointF>

#include "source/model/term/paintedterm.h"

class TermPositioner
{
public:
    virtual ~TermPositioner() = default;

    virtual std::optional<QPointF> preferredPositionFor(PaintedTerm::Ptr term) const = 0;
};
