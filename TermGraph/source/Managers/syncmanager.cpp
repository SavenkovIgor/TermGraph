#include "syncmanager.h"

SyncManager::SyncManager(GroupsManager *groupsManger,
                         NodesManager *nodesManager,
                         QObject *parent)
    : QObject(parent)
{
    this->groupsManager = groupsManger;
    this->nodesManager = nodesManager;
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
