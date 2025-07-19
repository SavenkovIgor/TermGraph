// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QAbstractListModel>
#include <QObject>

// BUG: on empty groups, edges didn't hide

#include "source/model/edge/paintededge.h"

class TermGroup;

class EdgesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit EdgesModel(QObject* parent = nullptr);

    enum Roles { Pt1 = 0, Pt2, IsSelected, EdgeType, EdgeSelection };

    Q_ENUM(Roles)

    void setGroup(std::optional<std::shared_ptr<TermGroup>> group);
    void clear();

    QHash<int, QByteArray> roleNames() const override;

    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void updateSelection();

private:
    std::optional<std::shared_ptr<TermGroup>> mGroup = std::nullopt;
    PaintedEdge::List mEdges;
};
