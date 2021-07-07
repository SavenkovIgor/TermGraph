/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#include "source/Model/Termin/termsmodel.h"

#include "source/Helpers/appstyle.h"

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
                                           {Roles::Color, "color"},
                                           {Roles::Weight, "weight"},
                                           {Roles::Term, "term"},
                                           {Roles::IsSelected, "isSelected"}};

    return roles;
}

int TermsModel::rowCount(const QModelIndex &parent) const { return mTerms.size(); }

QVariant TermsModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row >= mTerms.size() || row < 0)
        return QVariant();

    auto term = mTerms[row];

    switch (static_cast<Roles>(role)) {
    case Roles::Rect: return term->rect();
    case Roles::Radius: return term->cornerRadius();
    case Roles::Color: return nodeColor(mGroup.value()->termType(term), term->isSelectedAnyway());
    case Roles::Weight: return 0.1;
    case Roles::Term: return term->decoratedTerm();
    case Roles::IsSelected: return term->isSelectedAnyway();
    }

    Q_UNREACHABLE();
    return QVariant();
}

void TermsModel::updateSelection()
{
    emit dataChanged(index(0), index(mTerms.size() - 1), {Roles::Color, Roles::IsSelected});
}

QColor TermsModel::nodeColor(NodeType type, bool selected) const
{
    switch (type) {
    case NodeType::orphan: return selected ? AppStyle::Colors::Nodes::orphanSelected : AppStyle::Colors::Nodes::orphan;
    case NodeType::root: return selected ? AppStyle::Colors::Nodes::rootSelected : AppStyle::Colors::Nodes::root;
    case NodeType::endLeaf: return selected ? AppStyle::Colors::Nodes::leafSelected : AppStyle::Colors::Nodes::leaf;
    case NodeType::middleLeaf: return selected ? AppStyle::Colors::Nodes::leafSelected : AppStyle::Colors::Nodes::leaf;
    }

    assert(false); // must be unreachable
    return AppStyle::Colors::Nodes::orphan;
}
