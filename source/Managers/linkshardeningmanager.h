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
#include <QScopedPointer>
#include <QStringView>

#include "source/Helpers/handytypes.h"
#include "source/Helpers/link/linksstring.h"
#include "source/Model/Termin/nodegadgetwrapper.h"
#include "source/Model/Termin/paintedterm.h"
#include "source/Model/TerminGroup/termgroup.h"

class LinksHardeningManager : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool canMoveNext READ canMoveNext NOTIFY indexChanged);
    Q_PROPERTY(bool canMovePrev READ canMovePrev NOTIFY indexChanged);
    Q_PROPERTY(QString definitionWithHighlightedLink READ definitionWithHighlightedLink NOTIFY indexChanged);
    Q_PROPERTY(QString currentLinkText READ currentLinkText NOTIFY indexChanged);

public:
    using SearchResult     = std::tuple<QUuid, QString, int>;
    using SearchResultList = std::vector<SearchResult>;

    explicit LinksHardeningManager(QObject* parent = nullptr);

    enum Role { Uuid = Qt::UserRole, Text, Distance };

    // Model stuff
    QHash<int, QByteArray> roleNames() const final;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const final;

    int rowCount(const QModelIndex& parent = QModelIndex()) const final;

    Q_INVOKABLE void setGroup(TermGroup* group);
    Q_INVOKABLE void setTerm(NodeGadgetWrapper termWrapper);

    Q_INVOKABLE void prev();
    Q_INVOKABLE void next();

    Q_INVOKABLE void hardenLink(QUuid uuid);

    SearchResultList getNearestVariants(int limit = 4);

    Q_INVOKABLE NodeGadgetWrapper appliedReplacement();

signals:
    void indexChanged();

private slots:
    void updateNearestVariants();

private: // Methods
    Link::List currentLinks() const;
    Link       currentLink() const;

    bool isValidIndex() const;

    bool canMoveNext() const;
    bool canMovePrev() const;

    QString currentLinkText();
    QString definitionWithHighlightedLink() const;

    int linkCount() const;

    QString appliedLinkFixationText() const;

    static QString applyLinkUuids(QString stringWithLinks, QMap<int, QUuid> uuidsToApply);

private: // Members
    TermGroup*                  mCurrentGroup = nullptr;
    NodeGadgetWrapper           mCurrentTerm;
    QString                     mCurrentDefinition;
    QScopedPointer<LinksString> mLinksString;

    int mLinkIndex = -1;

    QMap<int, QUuid> mReplacePreparations;

    SearchResultList mLastNearestVariants;
};
