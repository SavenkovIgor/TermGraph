// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <vector>

#include "source/model/group/termpositioner.h"
#include "source/model/term/paintedterm.h"

class NodeVerticalStack
{
public:
    using List     = std::vector<NodeVerticalStack>;
    using NodePack = QPair<QPointF, PaintedTerm::List>;

    NodeVerticalStack(TermPositioner* termPositioner);
    ~NodeVerticalStack() = default;

    void addTerm(PaintedTerm::Ptr term);
    void placeTerms(QPointF centerPoint);

    QSizeF size() const;
    bool   hasNode(PaintedTerm::Ptr term) const;

    PaintedTerm::List nodes() const;

private:
    static std::vector<NodePack> getNodePacks(const PaintedTerm::List& terms, const TermPositioner* termPositioner);
    static void                  sortNodePacks(std::vector<NodePack>& pack);
    static PaintedTerm::List     flatNodePack(const std::vector<NodePack>& pack);

private: // Members
    PaintedTerm::List     mTerms;
    const TermPositioner* mTermPositioner = nullptr;
};
