// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QSqlRecord>

#include <CommonTools/HandyTypes.h>
#include <source/TermDataInterface/DataStorageInterface.h>
#include <source/TermDataInterface/TermData.h>

class TermTable
{
public:
    using RecordList = std::vector<QSqlRecord>;

    TermTable()  = default;
    ~TermTable() = default;

    // Table stuff
    void initTable();

    // Read
    bool             exist(const TermUuid& uuid) const;
    Result<TermData> term(const TermUuid& uuid) const;
    Result<TermData> term(const QString& term, const GroupUuid& uuid) const;

    Result<TermData::List> allTerms(const GroupUuid& uuid);
    RecordList             allLastEditRecords();

    // Modify
    Result<TermData> addTerm(const TermData& info);
    Result<TermData> updateTerm(const TermData&                      info,
                                DataStorageInterface::LastEditSource lastEditSource,
                                bool                                 checkLastEdit);
    Result<TermData> deleteTerm(const TermUuid& uuid);

private:
    TermUuid generateNewUuid();

    static QDateTime  now();
    Result<QDateTime> lastEdit(const TermUuid& uuid);

    static TermData createTermData(const QSqlRecord& record);
};
