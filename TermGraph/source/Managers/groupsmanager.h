/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

#include <QObject>

#include "source/Helpers/fsworks.h"
#include "source/Managers/nodesmanager.h"
#include "source/Model/TerminGroup/termgroup.h"
#include "source/Managers/notificationmanager.h"

class GroupsManager : public QObject
{
    Q_OBJECT
public:
    explicit GroupsManager(NodesManager *nodesMgr, QObject *parent = nullptr);

    TermGroup* createGroup(const QUuid groupUuid);

    Q_PROPERTY(bool hasAnyGroup READ getHasAnyGroup NOTIFY groupsListChanged)
    Q_PROPERTY(QStringList allUuidSorted READ getAllUuidStringsSortedByLastEdit NOTIFY groupsListChanged)

    Q_INVOKABLE bool isEmptyGroup(const QString& groupUuid);

signals:
    void groupsListChanged();

public slots:
    bool getHasAnyGroup() const;

    QStringList getAllGroupsNames(bool withAllVeiw = false);
    QList<QUuid> getAllUuidsSortedByLastEdit();
    QUuid        getLastEditedGroupUuid();

    QList <TermGroup*> getAllGroups();

    QString getGroupName(const QUuid& groupUuid) const;
    QUuid getGroupUuid(const QString& groupName) const;
    void updateGroupUuidNameMaps();

    QString getLastEditString(QUuid groupUuid);
    int getNodesCount(QUuid groupUuid);

    QStringList getGroupNames(const QList<QUuid> &groupUuids);

    void addNewGroup(const QString &name, const QString &comment);
    void deleteGroup(QString groupUuid);

    // Json
    void importGroupFromJsonFile(const QString& filename);
    void importGroupFromJsonString(const QString& rawJson);
    void importGroupFromJson(const QJsonDocument &json);

    QString getExportPath() const;
    void exportGrpToJson(QString groupUuid);
    void saveGroupInFolder(TermGroup *group);

    QJsonDocument getGroupForExport(const QUuid& groupUuid);

private:
    QStringList getAllUuidStringsSortedByLastEdit();

    bool isValidGroupJson(const QJsonDocument json);

    TermGroup* createGroup(const QString &groupName);

    QDateTime getLastEdit(QUuid groupUuid);

    NodesManager* nodesMgr;

    // Cache
    QMap<QUuid, QString> uuidToNames;
    QMap<QString, QUuid> namesToUuid;
};
