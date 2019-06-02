#ifndef SYNCMANAGER_H
#define SYNCMANAGER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

#include "groupsmanager.h"
#include "nodesmanager.h"

class SyncManager : public QObject
{
    Q_OBJECT

public:
    explicit SyncManager(GroupsManager *groupsManger,
                         NodesManager *nodesManager,
                         QObject *parent = nullptr);

    enum class ContentType { undefined = 0, groupsHashList };

    constexpr static auto contentTypeFieldName = "contentType";
    constexpr static auto versionFieldName = "version";
    constexpr static auto contentFieldName = "content";

    static bool isDataContainer(const QJsonDocument &doc);
    //signals:
    //public slots:

private:
    GroupsManager* groupsManager;
    NodesManager* nodesManager;

    static QString getContentTypeName(const ContentType& type);
    static ContentType getContentTypeByName(const QString& typeName);
};

#endif // SYNCMANAGER_H
