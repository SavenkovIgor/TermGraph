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

#pragma once

#include <QAbstractListModel>
#include <QObject>

#include "source/Model/TerminEdge/paintededge.h"

class EdgesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit EdgesModel(QObject* parent = nullptr);

    enum Roles { Pt1 = 0, Pt2, Color, IsSelected };

    Q_ENUM(Roles);

    void setEdges(PaintedEdge::List edges);
    void clear();

    QHash<int, QByteArray> roleNames() const override;

    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void updateSelection();

private:
    PaintedEdge::List mEdges;
};
