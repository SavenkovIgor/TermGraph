// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QLatin1String>
#include <QSqlQuery>
#include <QString>
#include <QtCore>

#include <source/CommonTools/GroupUuid.h>
#include <source/CommonTools/TermUuid.h>

#include <source/TermDataInterface/GroupSummary.h>
#include <source/TermDataInterface/TermData.h>

// Задача этого класса - формировать запросы
class SqlQueryBuilder
{
public:
    SqlQueryBuilder() = default;

    // AppConfig table
    QSqlQuery createAppConfigTable() const;
    QSqlQuery insertConfigParameter(const QString& parameter, const QString& value) const;
    QSqlQuery selectOneConfigParameter(const QString& parameter) const;
    QSqlQuery selectConfigParameter(const QString& parameter) const;
    QSqlQuery updateConfigParameter(const QString& parameter, const QString& newValue) const;

    // Groups table
    QSqlQuery createGroupsTable() const;
    QSqlQuery insertGroup(const GroupSummary& groupInfo) const;
    QSqlQuery selectGroup(const GroupUuid& uuid) const;
    QSqlQuery selectGroup(const QString& groupName) const;
    QSqlQuery selectOneGroup(const GroupUuid& uuid) const;
    QSqlQuery selectAllGroups();
    QSqlQuery updateGroup(const GroupSummary& groupInfo) const;
    QSqlQuery deleteGroup(const GroupUuid& uuid) const;

    // Terms table
    QSqlQuery createTermsTable() const;
    QSqlQuery insertTerm(const TermData& termInfo) const;
    QSqlQuery selectTerm(const TermUuid& uuid) const;
    QSqlQuery selectOneTerm(const TermUuid& uuid) const;
    QSqlQuery selectOneTerm(const QString& term, const GroupUuid& uuid) const;
    QSqlQuery selectAllTerms(const GroupUuid& uuid) const;
    QSqlQuery selectLastEdit(const TermUuid& uuid) const;
    QSqlQuery selectAllLastEditAndGroupUuid() const;
    QSqlQuery updateTerm(const TermData& termInfo) const;
    QSqlQuery deleteTerm(const TermUuid& uuid) const;

    // Common
    QSqlQuery loadQuery(const QString& queryPath) const;
    QSqlQuery recordsCount(const QString& tableName) const;
    QSqlQuery dropTable(const QString& tableName) const;

private: // Methods
    QString loadQueryString(const QString& queryPath) const;

    static QLatin1String connectionNameForCurrentThread();
    static QSqlDatabase  getDbForConnection(QLatin1String connectionName);
};
