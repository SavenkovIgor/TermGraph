// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQmlEngine>

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
