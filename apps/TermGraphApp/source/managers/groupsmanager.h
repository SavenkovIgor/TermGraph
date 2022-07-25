// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonDocument>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QUuid>

#include <CommonTools/HandyTypes.h>
#include <TermDataInterface/DataStorageInterface.h>

#include "source/managers/notifiyinterface.h"
#include "source/model/group/termgroup.h"

class GroupsManager : public QObject
{
    Q_OBJECT

public:
    explicit GroupsManager(std::unique_ptr<DataStorageInterface> dataStorage,
                           NotifyInterface&                      notifier,
                           QObject*                              parent = nullptr);

    Q_PROPERTY(bool hasAnyGroup READ getHasAnyGroup NOTIFY groupsListChanged)
    Q_PROPERTY(QStringList allUuidSorted READ getAllUuidStringsSortedByLastEdit NOTIFY groupsListChanged)

    Q_INVOKABLE bool    isEmptyGroup(const QString& groupUuid);
    Q_INVOKABLE QString getGroupName(const QUuid& groupUuid) const;
    Q_INVOKABLE QString getLastEditString(QUuid groupUuid);
    Q_INVOKABLE int     getNodesCount(QUuid groupUuid);

    Q_INVOKABLE void addNewGroup(const QString& name, const QString& comment);
    Q_INVOKABLE void deleteGroup(const QString& groupUuid);

    Q_INVOKABLE QString getExportPath() const;
    Q_INVOKABLE void    exportGrpToJson(const QString& groupUuid);

    TermGroup::OptPtr createGroup(const QUuid groupUuid);

    UuidList getAllUuidsSortedByLastEdit();

    void updateGroupUuidNameMaps();

    QJsonDocument getGroupForExport(const QUuid& groupUuid) const;

    // Json
    void importGroupFromJsonFile(const QString& filename);
    void importGroupFromJsonString(const QString& rawJson);
    void importGroup(const QJsonDocument& json);
    void importTerm(const QJsonObject& nodeJson);

    Q_INVOKABLE int dbVersion();

    // Nodes
    Q_INVOKABLE bool addNode(QJsonObject object);
    Q_INVOKABLE bool updateNode(const QJsonObject& object);
    Q_INVOKABLE void deleteNode(const QUuid uuid);

signals:
    void groupsListChanged();
    void groupAdded();
    void groupDeleted();
    void nodeChanged();

private: // Methods
    bool getHasAnyGroup() const;

    void saveGroupInFolder(TermGroup::OptPtr group);

    bool groupExist(const GroupUuid& uuid);

private: // Members
    QStringList getAllUuidStringsSortedByLastEdit();

    QDateTime getLastEdit(QUuid groupUuid);

    std::unique_ptr<DataStorageInterface> dataSource;
    NotifyInterface&      notifier;

    // Cache
    QMap<QUuid, QString> uuidToNames;
};
