// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QString>

class NotifyInterface
{
public:
    virtual ~NotifyInterface() = default;

    virtual void showInfo(const QString& info)       = 0;
    virtual void showWarning(const QString& warning) = 0;
    virtual void showError(const QString& error)     = 0;
    virtual void showDebug(const QString& debug)     = 0;
};
