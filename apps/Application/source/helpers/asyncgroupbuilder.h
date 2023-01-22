// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <functional>

#include <QThread>

#include "source/model/group/termgroup.h"

class AsyncGroupBuilder : public QThread
{
    Q_OBJECT

public:
    explicit AsyncGroupBuilder(QObject* parent = nullptr);

    void setAction(std::function<TermGroup::OptPtr()> func);

    TermGroup::OptPtr takeResult();

protected:
    void run() final;

private:
    std::function<TermGroup::OptPtr()> mAction;

    TermGroup::OptPtr mResultGroup = std::nullopt;
};
