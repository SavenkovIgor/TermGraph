// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

#include "source/managers/groupsmanager.h"
#include "source/managers/networkmanager.h"

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
