// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQmlEngine>

class EdgeSelection : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    enum class Type { None, Forward, Backward };
    Q_ENUM(Type)
};
