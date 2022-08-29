// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QAbstractListModel>
#include <QObject>

#include "source/model/group/termgroup.h"


class TermsModel : public QAbstractListModel
{
    Q_OBJECT

    enum Roles { Rect = 0, Radius, Weight, Term, IsSelected, Type };

public:
    explicit TermsModel(QObject* parent = nullptr);

    void setGroup(TermGroup::OptPtr group);
    void clear();

    QHash<int, QByteArray> roleNames() const override;

    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void updateSelection();

private: // Members
    TermGroup::OptPtr mGroup = std::nullopt;
    PaintedTerm::List mTerms;
};
