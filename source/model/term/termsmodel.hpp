// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QAbstractListModel>
#include <QObject>

class TermGroup;
class PaintedTerm;

class TermsModel : public QAbstractListModel
{
    Q_OBJECT

    enum Roles { Rect = 0, Radius, Weight, Term, TermAndSynonyms, Definition, IsSelected, Type, Area };

public:
    explicit TermsModel(QObject* parent = nullptr);

    void setGroup(std::optional<std::shared_ptr<TermGroup>> group);
    void clear();

    QHash<int, QByteArray> roleNames() const override;

    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void updateSelection();

private: // Members
    std::optional<std::shared_ptr<TermGroup>> mGroup = std::nullopt;
    std::vector<std::shared_ptr<PaintedTerm>> mTerms;
};
