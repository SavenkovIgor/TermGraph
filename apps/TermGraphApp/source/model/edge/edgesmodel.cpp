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

#include "source/model/edge/edgesmodel.h"

EdgesModel::EdgesModel(QObject *parent)
    : QAbstractListModel(parent)
{}

void EdgesModel::setGroup(TermGroup::OptPtr group)
{
    beginResetModel();
    mGroup = group;

    if (mGroup.has_value()) {
        mEdges = mGroup.value()->edgesForPaint();
    } else {
        mEdges.clear();
    }

    endResetModel();
}

void EdgesModel::clear() { setGroup(std::nullopt); }

QHash<int, QByteArray> EdgesModel::roleNames() const
{
    static QHash<int, QByteArray> roles = {{Roles::Pt1, "pt1"},
                                           {Roles::Pt2, "pt2"},
                                           {Roles::IsSelected, "isSelected"},
                                           {Roles::EdgeType, "edgeType"},
                                           {Roles::EdgeSelection, "edgeSelection"}};

    return roles;
}

int EdgesModel::rowCount(const QModelIndex &parent) const { return mEdges.size(); }

QVariant EdgesModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row >= mEdges.size() || row < 0)
        return QVariant();

    auto edge = mEdges[row];

    switch (static_cast<Roles>(role)) {
    case Roles::Pt1: return edge->rootPoint();
    case Roles::Pt2: return edge->leafPoint();
    case Roles::IsSelected: return edge->isSelected();
    case Roles::EdgeType: return static_cast<int>(edge->data().type);
    case Roles::EdgeSelection: return static_cast<int>(edge->data().selectionType);
    }

    Q_UNREACHABLE();
    return QVariant();
}

void EdgesModel::updateSelection() { emit dataChanged(index(0), index(mEdges.size() - 1), {Roles::IsSelected}); }
