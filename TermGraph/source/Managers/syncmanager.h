#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

#include "networkmanager.h"
#include "groupsmanager.h"
#include "nodesmanager.h"

class SyncManager : public QObject
{
    Q_OBJECT

public:
    explicit SyncManager(NetworkManager *networkManager,
                         GroupsManager *groupsManger,
                         NodesManager *nodesManager,
                         QObject *parent = nullptr);

    enum class ContentType { undefined = 0, groupsHashList };

    constexpr static auto contentTypeFieldName = "contentType";
    constexpr static auto versionFieldName = "version";
    constexpr static auto contentFieldName = "content";

    static bool isDataContainer(const QJsonDocument &doc);
    //signals:

public slots:
    void sendGroupByNetwork(const QString& groupUuid);

private:
    NetworkManager* networkManager;
    GroupsManager* groupsManager;
    NodesManager* nodesManager;

    static QString getContentTypeName(const ContentType& type);
    static ContentType getContentTypeByName(const QString& typeName);
};
