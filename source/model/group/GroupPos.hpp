// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <utility>
#include <variant>
#include <QMetaType>

#include "source/model/group/ForestPos.hpp"
#include "source/model/group/OrphanPos.hpp"

// {forestIndex, localPos}
using ForestsPos = std::pair<size_t, ForestPos>;

// Logical position of a term within a group.
// A term is either placed inside a forest (ForestsPos) or is an orphan (OrphanPos).
struct GroupPos {
    Q_GADGET
    Q_PROPERTY(bool      isOrphan    READ isOrphan    FINAL)
    Q_PROPERTY(size_t    forestIndex READ forestIndex FINAL)
    Q_PROPERTY(ForestPos forestPos   READ forestPos   FINAL)
    Q_PROPERTY(OrphanPos orphanPos   READ orphanPos   FINAL)

    std::variant<ForestsPos, OrphanPos> pos = ForestsPos{};

    bool isOrphan() const { return std::holds_alternative<OrphanPos>(pos); }

    size_t forestIndex() const
    {
        if (const auto* p = std::get_if<ForestsPos>(&pos))
            return p->first;
        return {};
    }

    ForestPos forestPos() const
    {
        if (const auto* p = std::get_if<ForestsPos>(&pos))
            return p->second;
        return {};
    }

    OrphanPos orphanPos() const
    {
        if (const auto* p = std::get_if<OrphanPos>(&pos))
            return *p;
        return {};
    }
};

Q_DECLARE_METATYPE(GroupPos)
