#ifndef GROUPSMANAGER_H
#define GROUPSMANAGER_H

#include <QObject>

#include "../glb.h"
#include "../Helpers/fsworks.h"
#include "./nodesmanager.h"
#include "./networkmanager.h"
#include "../Model/TerminGroup/termgroup.h"

class GroupsManager : public QObject
{
    Q_OBJECT
public:
    explicit GroupsManager(
            NodesManager* nodesMgr,
            NetworkManager* network,
            QObject *parent = nullptr);

    TermGroup* createGroup(const QUuid groupUuid);

signals:
    void groupsListChanged();

    void showInfo(QString info);
    void showWarning(QString warning);
    void showError(QString error);

public slots:

    bool hasAnyGroup() const;

    QStringList getAllGroupsNames(bool withAllVeiw = false);
    QList<QUuid> getAllUuidsSortedByLastEdit();
    QStringList getAllUuidStringsSortedByLastEdit();
    QList <TermGroup*> getAllGroups();

    QString getGroupName(QUuid groupUuid) const;
    QString getLastEditString(QUuid groupUuid);
    int getNodesCount(QUuid groupUuid);
    QUuid getGroupUuid(const QString &groupName);

    QStringList getGroupNames(const QList<QUuid> &groupUuids);

    void addNewGroup(const QString &name, const QString &comment);
    void deleteGroup(QString groupUuid);

    // Json
    void importGroupFromJsonFile(const QString& filename);
    void importGroupFromJson(const QString& rawJson);
    void importGroupFromJson(const QJsonDocument &json);

    QString getExportPath() const;
    void exportGrpToJson(QString groupUuid);
    void saveGroupInFolder(TermGroup *group);

    // Network
    void sendGroupByNetwork(const QString groupUuid);

private:
    bool isValidGroupJson(const QJsonDocument json);

    TermGroup* createGroup(const QString &groupName);


    QDateTime getLastEdit(QUuid groupUuid);

    NodesManager* nodesMgr;
    NetworkManager* network;
};

#endif  // GROUPSMANAGER_H
