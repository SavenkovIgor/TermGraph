// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQmlEngine>

#include "source/graph/Forest.hpp"

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

    static Type fromTermType(graph::NodeType type)
    {
        switch (type) {
        case graph::NodeType::Orphan: return Type::Orphan;
        case graph::NodeType::Root: return Type::Root;
        case graph::NodeType::EndLeaf: return Type::EndLeaf;
        case graph::NodeType::MiddleLeaf: return Type::MiddleLeaf;
        }
        Q_UNREACHABLE();
    }
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

    static Type from(const QString& str)
    {
        if (str == "phil")
            return Type::Philosophy;
        if (str == "math")
            return Type::Math;
        if (str == "phys")
            return Type::Physics;
        if (str == "chem")
            return Type::Chemistry;
        if (str == "bio")
            return Type::Biology;
        if (str == "med")
            return Type::Medicine;
        if (str == "neuro")
            return Type::Neuroscience;
        if (str == "soc")
            return Type::Sociology;
        if (str == "art")
            return Type::Art;
        if (str == "cs")
            return Type::ComputerScience;
        if (str == "econ")
            return Type::Economics;

        return Type::None;
    }

    static QString toString(Type type)
    {
        switch (type) {
        case Type::None: return "";
        case Type::Philosophy: return "phil";
        case Type::Math: return "math";
        case Type::Physics: return "phys";
        case Type::Chemistry: return "chem";
        case Type::Biology: return "bio";
        case Type::Medicine: return "med";
        case Type::Neuroscience: return "neuro";
        case Type::Sociology: return "soc";
        case Type::Art: return "art";
        case Type::ComputerScience: return "cs";
        case Type::Economics: return "econ";
        default: return "";
        }
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
