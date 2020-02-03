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

#include <QJsonObject>
#include <QObject>
#include <QSqlRecord>
#include <QString>
#include <QUuid>

#include "source/Managers/notificationmanager.h"
#include "source/Model/Termin/paintedterm.h"
#include "source/databaseWorks/database.h"

class NodesManager : public QObject
{
    Q_OBJECT

public:
    explicit NodesManager(QObject* parent = nullptr);

signals:
    void nodeChanged();

public slots:
    bool  correctNewNodeName(const QString& name, QUuid& groupUuid, bool showWarnings = true);
    QUuid getNodeUuidByNameAndGroup(const QString& name, QUuid& groupUuid) const;
    bool  hasNodeWithNameInGroup(const QString& name, QUuid& groupUuid) const;

    /*[[nodiscard]]*/ bool addNewNode(const QString& name,
                                      const QString& def,
                                      const QString& descr,
                                      const QString& exam,
                                      const QString& groupUuidString,
                                      const bool&    sendChangeSignal = true);

    /*[[nodiscard]]*/ bool changeNode(const QUuid&   nodeUuid,
                                      const QString& name,
                                      const QString& definition,
                                      const QString& description,
                                      const QString& example,
                                      const QString& groupUuidString,
                                      const bool&    sendChangeSignal = true);

    /*[[nodiscard]]*/ bool changeNode(const QUuid&   nodeUuid,
                                      const QString& name,
                                      const QString& definition,
                                      const QString& description,
                                      const QString& example,
                                      const QUuid&   groupUuid,
                                      const bool&    sendChangeSignal = true);
    void                   deleteNode(QUuid uuid);

    UuidList          getAllNodesUuidsInGroup(const QUuid& groupUuid);
    PaintedTerm::List getAllNodesForGroup(const QUuid& groupUuid);

    QDateTime getLastEdit(QUuid nodeUuid);

    void        importNodeFromJson(QJsonObject nodeJson, bool importIfGroupNotExist = false);
    QJsonObject getNodeJson(const QUuid& uuid);

private:
    bool correctGroupUuid(const QUuid& groupUuid, bool sendWarnings = true);
};
