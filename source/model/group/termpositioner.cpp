// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <optional>

#include <QPointF>

export module TermGroup.TermPositioner;

import PaintedTerm;

using namespace std;

export class TermPositioner
{
public:
    virtual ~TermPositioner() = default;

    virtual optional<QPointF> preferredPositionFor(PaintedTerm::Ptr term) const = 0;
};
