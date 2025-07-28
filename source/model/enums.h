// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQmlEngine>

#include "source/graph/NodeType.hpp"

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
    enum class Type { Orphan, Root, EndLeaf, MiddleLeaf, Ghost };
    Q_ENUM(Type)

    static Type fromTermType(graph::NodeType type);
};

class KnowledgeArea : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    enum class Type {
        None = 0,
        Philosophy,
        Math,
        Physics,
        Chemistry,
        Biology,
        Medicine,
        Neuroscience,
        Sociology,
        Art,
        ComputerScience,
        Economics
    };
    Q_ENUM(Type)

    static Type    from(const QString& str);
    static QString toString(Type type);
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
