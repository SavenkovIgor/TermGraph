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

#include <QJsonDocument>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QUuid>

#include "source/Helpers/fsworks.h"
#include "source/Managers/datastorageinterface.h"
#include "source/Managers/nodesmanager.h"
#include "source/Managers/notificationmanager.h"
#include "source/Model/TerminGroup/termgroup.h"

class GroupsManager : public QObject
{
    Q_OBJECT
public:
    explicit GroupsManager(DataStorageInterface& dataStorage, NodesManager* nodesMgr, QObject* parent = nullptr);

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

    TermGroup* createGroup(const QUuid groupUuid);

    QList<QUuid> getAllUuidsSortedByLastEdit();

    void updateGroupUuidNameMaps();

    QJsonDocument getGroupForExport(const QUuid& groupUuid) const;

    // Json
    void importGroupFromJsonFile(const QString& filename);
    void importGroupFromJsonString(const QString& rawJson);
    void importGroupFromJson(const QJsonDocument& json);

    Q_INVOKABLE int dbVersion();

signals:
    void groupsListChanged();
    void groupAdded();
    void groupDeleted();

private: // Methods
    bool getHasAnyGroup() const;

    QUuid getGroupUuid(const QString& groupName) const;

    void saveGroupInFolder(TermGroup* group);

private:
    QStringList getAllUuidStringsSortedByLastEdit();

    bool isValidGroupJson(const QJsonDocument json);

    QDateTime getLastEdit(QUuid groupUuid);

    NodesManager* nodesMgr;

    DataStorageInterface& dataStorage;

    // Cache
    QMap<QUuid, QString> uuidToNames;
    QMap<QString, QUuid> namesToUuid;
};
