// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

export module EdgeData;

import Enums.EdgeSelection;
import Enums.EdgeType;

export struct EdgeData
{
    EdgeSelection selectionType = EdgeSelection::None;
    EdgeType      type          = EdgeType::Termin;
};
