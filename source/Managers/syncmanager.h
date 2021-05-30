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
#include <QJsonObject>
#include <QObject>

#include "source/Managers/groupsmanager.h"
#include "source/Managers/networkmanager.h"

class SyncManager : public QObject
{
    Q_OBJECT

public:
    explicit SyncManager(NetworkManager* networkManager, GroupsManager* groupsManger, QObject* parent = nullptr);

    enum class ContentType { undefined = 0, groupsHashList };

    constexpr static auto contentTypeFieldName = "contentType";
    constexpr static auto versionFieldName     = "version";
    constexpr static auto contentFieldName     = "content";

    static bool isDataContainer(const QJsonDocument& doc);

    Q_INVOKABLE void sendGroupByNetwork(const QUuid groupUuid);

private:
    NetworkManager*      mNetworkManager;
    const GroupsManager* mGroupsManager;

    static QString     getContentTypeName(const ContentType& type);
    static ContentType getContentTypeByName(const QString& typeName);
};
