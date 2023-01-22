// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "source/model/enums.h"

struct EdgeData
{
    EdgeSelection::Type selectionType = EdgeSelection::Type::None;
    EdgeType::Type      type          = EdgeType::Type::Termin;
};
