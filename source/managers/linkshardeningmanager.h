// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <memory>

#include <QAbstractListModel>
#include <QStringView>

#include "source/model/group/qttermgroup.h"
#include "source/model/term/termdatawrapper.h"

class TermGroup;
class LinksString;

class LinksHardeningManager : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool canMoveNext READ canMoveNext NOTIFY indexChanged FINAL)
    Q_PROPERTY(bool canMovePrev READ canMovePrev NOTIFY indexChanged FINAL)
    Q_PROPERTY(QString definitionWithHighlightedLink READ definitionWithHighlightedLink NOTIFY indexChanged FINAL)
    Q_PROPERTY(QString currentLinkText READ currentLinkText NOTIFY indexChanged FINAL)

public:
    using SearchResult     = std::tuple<QUuid, QString, int>;
    using SearchResultList = std::vector<SearchResult>;

    explicit LinksHardeningManager(QObject* parent = nullptr);
    ~LinksHardeningManager() override;

    enum Role { Uuid = Qt::UserRole, Text, Distance };

    // Model stuff
    QHash<int, QByteArray> roleNames() const final;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const final;

    int rowCount(const QModelIndex& parent = QModelIndex()) const final;

    Q_INVOKABLE void setGroup(QtTermGroup* group);
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
    bool isValidIndex() const;

    bool canMoveNext() const;
    bool canMovePrev() const;

    QString currentLinkText();
    QString definitionWithHighlightedLink() const;

    int linkCount() const;

    QString appliedLinkFixationText() const;

    static QString applyLinkUuids(QString stringWithLinks, QMap<int, QUuid> uuidsToApply);

private: // Members
    std::shared_ptr<TermGroup>   mCurrentGroup = nullptr;
    TermDataWrapper              mCurrentTerm;
    QString                      mCurrentDefinition;
    std::unique_ptr<LinksString> mLinksString;

    // TODO: Make size type
    int mLinkIndex = -1;

    QMap<int, QUuid> mReplacePreparations;

    SearchResultList mLastNearestVariants;
};
