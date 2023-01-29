// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/model/term/termsmodel.h"

#include <CommonTools/HandyTypes.h>

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
                                           {Roles::Definition, "definition"},
                                           {Roles::IsSelected, "isSelected"},
                                           {Roles::Type, "type"}};

    return roles;
}

int TermsModel::rowCount([[maybe_unused]] const QModelIndex &parent) const { return mTerms.size(); }

QVariant TermsModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row >= mTerms.size() || row < 0)
        return QVariant();

    auto term = mTerms[row];

    switch (static_cast<Roles>(role)) {
    case Roles::Rect: return term->rect();
    case Roles::Radius: return term->cornerRadius();
    case Roles::Weight: return 0.1;
    case Roles::Term: return term->term();
    case Roles::Definition: return term->definition();
    case Roles::IsSelected: return term->isSelectedAnyway();
    case Roles::Type: return asInt(mGroup.value()->termType(term));
    }

    Q_UNREACHABLE();
    return QVariant();
}

void TermsModel::updateSelection() { emit dataChanged(index(0), index(mTerms.size() - 1), {Roles::IsSelected}); }
