// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/model/term/graphterm.h"

GraphTerm::GraphTerm(const TermData& info)
    : Node<TermData>(info)
    , mDataCache(info)
{}

const TermDataCache& GraphTerm::cache() const { return mDataCache; }

void GraphTerm::addSynonym(const QString& synonym) { mDataCache.addSynonym(synonym); }
