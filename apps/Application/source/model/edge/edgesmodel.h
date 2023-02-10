// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QAbstractListModel>
#include <QObject>

#include "source/model/group/termgroup.h"

// BUG: on empty groups, edges didnt hide

class EdgesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit EdgesModel(QObject* parent = nullptr);

    enum Roles { Pt1 = 0, Pt2, IsSelected, EdgeType, EdgeSelection };

    Q_ENUM(Roles)

    void setGroup(TermGroup::OptPtr group);
    void clear();

    QHash<int, QByteArray> roleNames() const override;

    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void updateSelection();

private:
    TermGroup::OptPtr mGroup = std::nullopt;
    PaintedEdge::List mEdges;
};
