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

#include <QJsonObject>
#include <QObject>
#include <QSqlRecord>
#include <QString>
#include <QUuid>

#include "source/Managers/datastorageinterface.h"
#include "source/Managers/notificationmanager.h"
#include "source/Model/Termin/paintedterm.h"

class NodesManager : public QObject
{
    Q_OBJECT

public:
    explicit NodesManager(DataStorageInterface& dataStorage, QObject* parent = nullptr);

signals:
    void nodeChanged();

public slots:

    bool addNewNode(const QString& name,
                    const QString& def,
                    const QString& descr,
                    const QString& exam,
                    const QString& groupUuidString,
                    const bool&    sendChangeSignal = true);

    bool changeNode(const QUuid&   nodeUuid,
                    const QString& name,
                    const QString& definition,
                    const QString& description,
                    const QString& example,
                    const QString& groupUuidString,
                    const bool&    sendChangeSignal = true);

    void deleteNode(QUuid uuid);

    UuidList          getAllNodesUuidsInGroup(const QUuid& groupUuid);
    PaintedTerm::List getAllNodesForGroup(const QUuid& groupUuid);

    QDateTime getLastEdit(QUuid nodeUuid);

    void        importNodeFromJson(const QJsonObject& nodeJson, bool importIfGroupNotExist = false);
    QJsonObject getNodeJson(const QUuid& uuid);

private:
    DataStorageInterface& dataStorage;

    bool correctGroupUuid(const QUuid& groupUuid);
    bool correctNewNodeName(const QString& name, QUuid& groupUuid);
};
