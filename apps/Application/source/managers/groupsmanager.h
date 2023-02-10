// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonDocument>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QUuid>

#include <CommonTools/HandyTypes.h>

#include "source/dataprovider.h"
#include "source/managers/notifyinterface.h"
#include "source/model/group/termgroup.h"

class GroupsManager : public QObject
{
    Q_OBJECT

public:
    explicit GroupsManager(
                           NotifyInterface&                      notifier,
                           QObject*                              parent = nullptr);

    Q_PROPERTY(bool hasAnyGroup READ getHasAnyGroup NOTIFY groupsListChanged FINAL)
    Q_PROPERTY(QStringList allUuidSorted READ getAllUuidStringsSortedByLastEdit NOTIFY groupsListChanged FINAL)

    Q_INVOKABLE bool    isEmptyGroup(const QString& groupUuid);
    Q_INVOKABLE QString getGroupName(const QUuid& groupUuid) const;
    Q_INVOKABLE QString getLastEditString(QUuid groupUuid);
    Q_INVOKABLE int     getNodesCount(QUuid groupUuid);

    Q_INVOKABLE void addNewGroup(const QString& name, const QString& comment);
    Q_INVOKABLE void deleteGroup(const QString& groupUuid);

    Q_INVOKABLE QString getExportPath() const;
    Q_INVOKABLE void    exportGrpToJson(const QString& groupUuid);

    Q_INVOKABLE void init();

    void loadGroup(const GroupUuid& uuid);
    TermGroup::OptPtr createGroup(Opt<GroupUuid> uuid = std::nullopt);

    GroupUuid::List getAllUuidsSortedByLastEdit();

    void requestGroupExport(const QUuid& groupUuid);

    // Json
    void importGroupFromJsonFile(const QString& filename);
    void importGroupFromJsonString(const QString& rawJson);
    void importGroup(QJsonObject json);
    void importTerm(const QJsonObject& nodeJson);

    Q_INVOKABLE int dbVersion();

    // Nodes
    Q_INVOKABLE void addNode(QJsonObject object);
    Q_INVOKABLE void updateNode(const QJsonObject& object);
    Q_INVOKABLE void deleteNode(const QUuid uuid);

signals:
    void groupListLoaded();
    void groupLoaded();

    void groupsListChanged();
    void termChanged();

    void groupAdded(GroupSummary data);
    void groupUpdated(GroupSummary data);
    void groupDeleted(GroupUuid uuid);

    void termAdded(TermData data);
    void termUpdated(TermData data);
    void termDeleted(TermUuid uuid);

    void exportGroupReady(QJsonDocument doc);

private slots:
    void showError(int code);

private: // Methods
    bool getHasAnyGroup() const;

    void saveGroupInFolder(TermGroup::OptPtr group);

    bool groupExist(const GroupUuid& uuid);

private: // Members
    QStringList getAllUuidStringsSortedByLastEdit();

    QDateTime getLastEdit(QUuid groupUuid);

    DataProvider provider;
    NotifyInterface&      notifier ;
};
