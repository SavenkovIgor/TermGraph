// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

// For coordinate types
enum class CoordType {
    zeroPoint, // Rect located in zero point
    local,     // Local coordinate system
    scene      // Scene coordinate system
};

class NodeType
{
    Q_GADGET

public:
    enum class Type { Orphan, Root, EndLeaf, MiddleLeaf };
    Q_ENUM(Type)
};

class EdgeType
{
    Q_GADGET

public:
    enum class Type { Standart, Termin, TerminHardLink, Description, Broken, Redundant };
    Q_ENUM(Type)
};

class EdgeSelection
{
    Q_GADGET
public:
    enum class Type { None, Forward, Backward };
    Q_ENUM(Type)
};
