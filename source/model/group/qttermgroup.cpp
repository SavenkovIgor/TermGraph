// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/model/group/qttermgroup.h"

#include "source/model/group/termgroup.h"

QtTermGroup::QtTermGroup(const TermGroup::Ptr termGroup, QObject* parent)
    : QObject(parent)
    , mTermGroup(termGroup)
{
}

QString QtTermGroup::qmlUuid() const { return mTermGroup->uuid().toString(); }

QString QtTermGroup::name() const { return mTermGroup->name(); }
