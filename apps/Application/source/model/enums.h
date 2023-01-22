// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQmlEngine>

#include <Graph/Forest.hpp>

// For coordinate types
enum class CoordType {
    zeroPoint, // Rect located in zero point
    local,     // Local coordinate system
    scene      // Scene coordinate system
};

class NodeType : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    enum class Type { Orphan, Root, EndLeaf, MiddleLeaf };
    Q_ENUM(Type)

    static Type fromTermType(graph::NodeType type)
    {
        switch (type) {
        case graph::NodeType::Orphan:     return Type::Orphan;
        case graph::NodeType::Root:       return Type::Root;
        case graph::NodeType::EndLeaf:    return Type::EndLeaf;
        case graph::NodeType::MiddleLeaf: return Type::MiddleLeaf;
        }
        Q_UNREACHABLE();
    }
};

class EdgeType : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    enum class Type { Standart, Termin, TerminHardLink, Description, Broken, Redundant };
    Q_ENUM(Type)
};

class EdgeSelection : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    enum class Type { None, Forward, Backward };
    Q_ENUM(Type)
};
