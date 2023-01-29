// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/model/edge/edgesmodel.h"

#include <CommonTools/HandyTypes.h>

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

int EdgesModel::rowCount([[maybe_unused]] const QModelIndex &parent) const { return mEdges.size(); }

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
    case Roles::EdgeType: return asInt(edge->data().type);
    case Roles::EdgeSelection: return asInt(edge->data().selectionType);
    }

    Q_UNREACHABLE();
    return QVariant();
}

void EdgesModel::updateSelection() { emit dataChanged(index(0), index(mEdges.size() - 1), {Roles::IsSelected}); }
