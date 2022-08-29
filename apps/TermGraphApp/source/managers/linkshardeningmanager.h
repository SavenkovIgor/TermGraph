// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QAbstractListModel>
#include <QScopedPointer>
#include <QStringView>

#include <CommonTools/HandyTypes.h>

#include "source/helpers/link/linksstring.h"
#include "source/model/group/termgroup.h"
#include "source/model/term/termdatawrapper.h"


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
    Q_INVOKABLE void setTerm(TermDataWrapper termWrapper);

    Q_INVOKABLE void prev();
    Q_INVOKABLE void next();

    Q_INVOKABLE void hardenLink(QUuid uuid);

    SearchResultList getNearestVariants(int limit = 4);

    Q_INVOKABLE TermDataWrapper appliedReplacement();

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
    TermDataWrapper             mCurrentTerm;
    QString                     mCurrentDefinition;
    QScopedPointer<LinksString> mLinksString;

    int mLinkIndex = -1;

    QMap<int, QUuid> mReplacePreparations;

    SearchResultList mLastNearestVariants;
};
