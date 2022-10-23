// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

#include <TermDataInterface/DataStorageInterface.h>
#include <TermDataInterface/GroupData.h>
#include <TermDataInterface/TermData.h>

class DataProvider : public QObject
{
    Q_OBJECT

public:
    explicit DataProvider(QObject *parent = nullptr);

    bool isReady() const;
    int dbVersion() const;
    Opt<GroupUuid> currentGroup() const;
    void loadGroups();
    void loadGroup(GroupUuid uuid);

    const GroupData::List& groups() const;
    Opt<GroupData> group(GroupUuid uuid) const;

    void addGroup(const GroupData& info);
    void updateGroup(const GroupData& info);
    void deleteGroup(const GroupUuid& uuid);

    const TermData::List& terms() const;
    Opt<TermData>         term(const QString& definition, GroupUuid uuid) const;

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

    void groupAdded(GroupData data);
    void groupUpdated(GroupData data);
    void groupDeleted(GroupUuid uuid);

    void termAdded(TermData data);
    void termUpdated(TermData data);
    void termDeleted(TermUuid uuid);

    void showError(int errorCode);

    void exportGroupReady(QJsonDocument doc);

private:
    std::unique_ptr<DataStorageInterface> dataStorage;

    Opt<GroupUuid> mCurrentGroup;
    Opt<GroupData::List> mGroups;
    TermData::List mTerms;
};
