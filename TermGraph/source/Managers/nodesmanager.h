#ifndef NODESMANAGER_H
#define NODESMANAGER_H

#include <QObject>
#include <QString>
#include <QUuid>
#include <QSqlRecord>
#include <QJsonObject>

#include "../databaseWorks/database.h"
#include "../Model/Termin/paintedterm.h"

class NodesManager : public QObject
{
    Q_OBJECT

public:
    explicit NodesManager(QObject *parent = nullptr);

signals:
    void nodeChanged();

    void showInfo(QString info);
    void showWarning(QString warning);
    void showError(QString error);

public slots:
    bool correctNewNodeName(const QString& name, QUuid& groupUuid, bool showWarnings = true);
    QUuid getNodeUuidByNameAndGroup(const QString& name, QUuid& groupUuid) const;
    bool hasNodeWithNameInGroup(const QString& name, QUuid& groupUuid) const;

    /*[[nodiscard]]*/ bool addNewNode(
            const QString& name,
            const QString& forms,
            const QString& def,
            const QString& descr,
            const QString& exam,
            const QString& groupUuidString,
            const bool& sendChangeSignal = true);

    /*[[nodiscard]]*/ bool changeNode(
            const QUuid& nodeUuid,
            const QString& name,
            const QString& forms,
            const QString& definition,
            const QString& description,
            const QString& example,
            const QString& groupUuidString,
            const bool& sendChangeSignal = true);

    /*[[nodiscard]]*/ bool changeNode(
            const QUuid& nodeUuid,
            const QString& name,
            const QString& forms,
            const QString& definition,
            const QString& description,
            const QString& example,
            const QUuid& groupUuid,
            const bool& sendChangeSignal = true);

    void deleteNode(QUuid uuid);

    QList<QUuid> getAllNodesUuidsInGroup(QUuid groupUuid);
    PaintedTerm::List getAllNodesForGroup(QUuid groupUuid);

    QDateTime getLastEdit(QUuid nodeUuid);

    void importNodeFromJson(QJsonObject nodeObject);

private:
    bool correctGroupUuid(const QUuid& groupUuid, bool sendWarnings = true);
    QSqlRecord getNodeSqlRecord(QUuid nodeUuid);
};

#endif  // NODESMANAGER_H
