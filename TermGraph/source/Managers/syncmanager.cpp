#include "syncmanager.h"

SyncManager::SyncManager(NetworkManager *networkManager,
                         GroupsManager *groupsManger,
                         NodesManager *nodesManager,
                         QObject *parent)
    : QObject(parent)
{
    this->networkManager = networkManager;
    this->groupsManager = groupsManger;
    this->nodesManager = nodesManager;

    connect(this->networkManager,
            &NetworkManager::newSyncGroup,
            this->groupsManager,
            &GroupsManager::importGroupFromJsonString);
}

bool SyncManager::isDataContainer(const QJsonDocument &doc)
{
    bool hasContentType = false;
    bool hasVersion = false;
    bool hasContent = false;

    auto docObject = doc.object();

    hasContentType = docObject.contains(contentTypeFieldName);
    hasVersion = docObject.contains(versionFieldName);
    hasContent = docObject.contains(contentFieldName);

    return hasContentType && hasVersion && hasContent;
}

void SyncManager::sendGroupByNetwork(const QString &groupUuid)
{
    if (auto group = groupsManager->createGroup(QUuid(groupUuid))) {
        networkManager->sendGroup(group->getJsonDoc());
        group->deleteLater();
//        delete group; // TODO: Проверить, почему удаление вызывает ошибку
    }
}

QString SyncManager::getContentTypeName(const SyncManager::ContentType &type)
{
    switch (type) {
    case ContentType::groupsHashList:
        return "groupsHashList";
    case ContentType::undefined:
        return "";
    }

    return "";
}

SyncManager::ContentType SyncManager::getContentTypeByName(const QString &typeName)
{
    if (typeName == "groupsHashList") {
        return ContentType::groupsHashList;
    }

    return ContentType::undefined;
}
