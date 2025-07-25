// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/model/term/termsmodel.h"

import TermGroup;
import CommonTools.HandyTypes;
import PaintedTerm;

TermsModel::TermsModel(QObject *parent)
    : QAbstractListModel(parent)
{}

void TermsModel::setGroup(TermGroup::OptPtr group)
{
    beginResetModel();
    mGroup = group;

    if (mGroup.has_value()) {
        mTerms = mGroup.value()->terms();
    } else {
        mTerms.clear();
    }

    endResetModel();
}

void TermsModel::clear() { setGroup(std::nullopt); }

QHash<int, QByteArray> TermsModel::roleNames() const
{
    static QHash<int, QByteArray> roles = {{Roles::Rect, "rect"},
                                           {Roles::Radius, "radius"},
                                           {Roles::Weight, "weight"},
                                           {Roles::Term, "term"},
                                           {Roles::TermAndSynonyms, "termAndSynonyms"},
                                           {Roles::Definition, "definition"},
                                           {Roles::IsSelected, "isSelected"},
                                           {Roles::Type, "type"},
                                           {Roles::Area, "area"}};

    return roles;
}

int TermsModel::rowCount([[maybe_unused]] const QModelIndex &parent) const { return asInt(mTerms.size()); }

QVariant TermsModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row >= asInt(mTerms.size()) || row < 0) {
        return {};
    }

    auto term = mTerms[PaintedTerm::asListSize(row)];

    switch (static_cast<Roles>(role)) {
    case Roles::Rect: return term->rect();
    case Roles::Radius: return term->cornerRadius();
    case Roles::Weight: return 0.1;
    case Roles::Term: return term->term();
    case Roles::TermAndSynonyms: return term->cache().termAndSynonyms();
    case Roles::Definition: return term->definition();
    case Roles::IsSelected: return term->isSelectedAnyway();
    case Roles::Type: return asInt(mGroup.value()->termType(term));
    case Roles::Area: return asInt(term->cache().area());
    }

    Q_UNREACHABLE();
    return {};
}

void TermsModel::updateSelection() { emit dataChanged(index(0), index(asInt(mTerms.size()) - 1), {Roles::IsSelected}); }
