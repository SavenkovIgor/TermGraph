// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QSqlRecord>

#include "source/CommonTools/HandyTypes.h"
#include "source/TermDataInterface/DataStorageInterface.h"
#include "source/TermDataInterface/TermData.h"

class TermTable
{
public:
    using RecordList = std::vector<QSqlRecord>;

    TermTable()  = default;
    ~TermTable() = default;

    // Table stuff
    void initTable();

    // Read
    bool               exist(const TermUuid& uuid) const;
    Expected<TermData> term(const TermUuid& uuid) const;
    Expected<TermData> term(const QString& term, const GroupUuid& uuid) const;

    Expected<TermData::List> allTerms(const GroupUuid& uuid);
    RecordList               allLastEditRecords();

    // Modify
    Expected<TermData> addTerm(const TermData& info);
    Expected<TermData> updateTerm(const TermData&                      info,
                                  DataStorageInterface::LastEditSource lastEditSource,
                                  bool                                 checkLastEdit);
    Expected<TermData> deleteTerm(const TermUuid& uuid);

private:
    TermUuid generateNewUuid();

    static QDateTime    now();
    Expected<QDateTime> lastEdit(const TermUuid& uuid);

    static TermData createTermData(const QSqlRecord& record);
};
