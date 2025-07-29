// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "source/enums/edge_selection.h"
#include "source/enums/edge_type.h"

struct EdgeData
{
    EdgeSelection selectionType = EdgeSelection::None;
    EdgeType      type          = EdgeType::Termin;
};
