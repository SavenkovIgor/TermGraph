// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/asyncgroupbuilder.h"

AsyncGroupBuilder::AsyncGroupBuilder(QObject* parent)
    : QThread(parent)
{}

void AsyncGroupBuilder::setAction(std::function<TermGroup::OptPtr()> func) { mAction = func; }

TermGroup::OptPtr AsyncGroupBuilder::takeResult()
{
    auto ret     = mResultGroup;
    mResultGroup = std::nullopt;
    return ret;
}

void AsyncGroupBuilder::run() { mResultGroup = mAction(); }
