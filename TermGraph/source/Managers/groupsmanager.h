#ifndef GROUPSMANAGER_H
#define GROUPSMANAGER_H

#include <QObject>

#include "../Helpers/fsworks.h"
#include "./nodesmanager.h"
#include "../Model/TerminGroup/termgroup.h"

class GroupsManager : public QObject
{
    Q_OBJECT
public:
    explicit GroupsManager(NodesManager *nodesMgr, QObject *parent = nullptr);

    TermGroup* createGroup(const QUuid groupUuid);

    Q_PROPERTY(bool hasAnyGroup READ getHasAnyGroup NOTIFY groupsListChanged)
    Q_PROPERTY(QStringList allUuidSorted READ getAllUuidStringsSortedByLastEdit NOTIFY groupsListChanged)

signals:
    void groupsListChanged();

    void showInfo(QString info);
    void showWarning(QString warning);
    void showError(QString error);

public slots:
    bool getHasAnyGroup() const;

    QStringList getAllGroupsNames(bool withAllVeiw = false);
    QList<QUuid> getAllUuidsSortedByLastEdit();
    QStringList getAllUuidStringsSortedByLastEdit();
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

private:
    bool isValidGroupJson(const QJsonDocument json);

    TermGroup* createGroup(const QString &groupName);

    QDateTime getLastEdit(QUuid groupUuid);

    NodesManager* nodesMgr;

    // Cache
    QMap<QUuid, QString> uuidToNames;
    QMap<QString, QUuid> namesToUuid;
};

#endif  // GROUPSMANAGER_H
