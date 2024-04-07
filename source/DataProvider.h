// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

#include "source/TermDataInterface/DataStorageInterface.h"
#include "source/TermDataInterface/GroupSummary.h"
#include "source/TermDataInterface/TermData.h"

class DataProvider : public QObject
{
    Q_OBJECT

public:
    explicit DataProvider(QObject* parent = nullptr);

    bool           isReady() const;
    int            dbVersion() const;
    std::optional<GroupUuid> currentGroup() const;
    void           loadGroups();
    void           loadGroup(GroupUuid uuid);

    const GroupSummary::List& groups() const;
    std::optional<GroupSummary> group(GroupUuid uuid) const;

    void addGroup(const GroupSummary& info);
    void updateGroup(const GroupSummary& info);
    void deleteGroup(const GroupUuid& uuid);

    const TermData::List& terms() const;
    std::optional<TermData> term(const QString& definition, GroupUuid uuid) const;

    void addTerm(const TermData& data);
    void updateTerm(const TermData&                      data,
                    DataStorageInterface::LastEditSource lastEditSource,
                    bool                                 checkLastEdit = true);
    void deleteTerm(const TermUuid& uuid);

    void importTerm(const TermData& data);
    void requestGroupExport(const GroupUuid& uuid);

signals:
    void groupListLoaded();
    void groupLoaded();

    void groupAdded(GroupSummary data);
    void groupUpdated(GroupSummary data);
    void groupDeleted(GroupUuid uuid);

    void termAdded(TermData data);
    void termUpdated(TermData data);
    void termDeleted(TermUuid uuid);

    void showError(int errorCode);

    void exportGroupReady(QJsonDocument doc);

private:
    std::unique_ptr<DataStorageInterface> dataStorage;

    std::optional<GroupUuid>          mCurrentGroup;
    std::optional<GroupSummary::List> mGroups;
    TermData::List          mTerms;
};
