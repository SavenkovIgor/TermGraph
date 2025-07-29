// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "source/enums/qml_edge_selection.h"
#include "source/model/enums.h"

struct EdgeData
{
    QmlEdgeSelection::Type selectionType = QmlEdgeSelection::Type::None;
    EdgeType::Type         type          = EdgeType::Type::Termin;
};
