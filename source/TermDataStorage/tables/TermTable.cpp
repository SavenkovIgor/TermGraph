// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QSqlRecord>

#include "source/CommonTools/HandyTypes.h"
#include "source/TermDataInterface/DataStorageInterface.h"
#include "source/TermDataInterface/TermData.h"
#include "source/TermDataStorage/DbTools.h"
#include "source/TermDataStorage/SqlQueryBuilder.h"

export module TermTable;

export class TermTable
{
public:
    using RecordList = std::vector<QSqlRecord>;

    TermTable()  = default;
    ~TermTable() = default;

    // Table stuff
    void initTable() { DbTools::start(SqlQueryBuilder().createTermsTable()); }

    // Read
    bool exist(const TermUuid& uuid) const
    {
        auto query = SqlQueryBuilder().selectOneTerm(uuid);
        DbTools::start(query);

        if (!query.next()) {
            return false;
        }

        auto count = query.record().value("COUNT( * )").toInt();
        return count > 0;
    }

    Expected<TermData> term(const TermUuid& uuid) const
    {
        if (!exist(uuid)) {
            return std::unexpected(ErrorCode::TermUuidNotFound);
        }

        auto query = SqlQueryBuilder().selectTerm(uuid);
        DbTools::start(query);

        auto record = DbTools::getRecord(std::move(query));
        return createTermData(record);
    }

    Expected<TermData> term(const QString& term, const GroupUuid& uuid) const
    {
        if (term.simplified().isEmpty()) {
            return std::unexpected(ErrorCode::TermEmpty);
        }

        auto query = SqlQueryBuilder().selectOneTerm(term, uuid);
        DbTools::start(query);

        if (query.next()) {
            return createTermData(query.record());
        }

        return std::unexpected(ErrorCode::TermUuidNotFound);
    }

    Expected<TermData::List> allTerms(const GroupUuid& uuid)
    {
        TermData::List ret;

        auto query = SqlQueryBuilder().selectAllTerms(uuid);
        DbTools::start(query);

        auto records = DbTools::getAllRecords(std::move(query));

        for (auto& record : records) {
            TermData info = createTermData(record);
            ret.push_back(std::move(info));
        }

        return ret;
    }

    RecordList allLastEditRecords()
    {
        auto query = SqlQueryBuilder().selectAllLastEditAndGroupUuid();
        DbTools::start(query);
        return DbTools::getAllRecords(std::move(query));
    }

    // Modify
    Expected<TermData> addTerm(const TermData& info)
    {
        // Generate new uuid if current is empty
        auto tUuid = info.uuid.value_or(generateNewUuid());

        if (exist(tUuid)) {
            return std::unexpected(ErrorCode::TermUuidAlreadyExist);
        }

        if (info.term.simplified().isEmpty()) {
            return std::unexpected(ErrorCode::TermEmpty);
        }

        if (term(info.term, info.groupUuid)) {
            return std::unexpected(ErrorCode::TermAlreadyExist);
        }

        TermData termInfo = info;

        termInfo.uuid = tUuid;

        if (termInfo.lastEdit.isNull()) {
            termInfo.lastEdit = now();
        }

        auto query = SqlQueryBuilder().insertTerm(termInfo);
        DbTools::start(query);

        return termInfo;
    }

    Expected<TermData> updateTerm(const TermData&                      info,
                                  DataStorageInterface::LastEditSource lastEditSource,
                                  bool                                 checkLastEdit)

    {
        if (!info.uuid) {
            return std::unexpected(ErrorCode::TermUuidInvalid);
        }

        if (!exist(*info.uuid)) {
            return std::unexpected(ErrorCode::TermUuidNotFound);
        }

        if (checkLastEdit) {
            const auto currentLastEdit = lastEdit(*info.uuid).value();
            const auto newLastEdit     = info.lastEdit;
            if (currentLastEdit > newLastEdit) { // If db version is fresher, do nothing
                return std::unexpected(ErrorCode::NewerTermVersionFound);
            }
        }

        TermData nodeContainer = info;

        if (lastEditSource == DataStorageInterface::Now) {
            nodeContainer.lastEdit = now();
        }

        DbTools::start(SqlQueryBuilder().updateTerm(nodeContainer));

        return nodeContainer;
    }

    Expected<TermData> deleteTerm(const TermUuid& uuid)
    {
        if (auto termData = term(uuid)) {
            DbTools::start(SqlQueryBuilder().deleteTerm(uuid));
            return termData.value();
        }

        return std::unexpected(ErrorCode::TermUuidNotFound);
    }

private:
    TermUuid generateNewUuid()
    {
        for (;;) {
            auto uuid = TermUuid::generate();
            if (!exist(uuid)) {
                return uuid;
            }
        }
    }

    static QDateTime now() { return QDateTime::currentDateTimeUtc(); }

    Expected<QDateTime> lastEdit(const TermUuid& uuid)
    {
        if (!exist(uuid)) {
            return std::unexpected(ErrorCode::TermUuidNotFound);
        }

        auto query = SqlQueryBuilder().selectLastEdit(uuid);
        DbTools::start(query);

        auto record = DbTools::getRecord(std::move(query));

        auto field = record.value("lastEdit").toString();

        if (field.isEmpty()) {
            return QDateTime();
        }

        return QDateTime::fromString(field, Qt::ISODate);
    }

    static TermData createTermData(const QSqlRecord& record)
    {
        auto gUuid = GroupUuid::from(record.value("groupUuid").toString());

        assert(gUuid);

        TermData info{
            .uuid          = TermUuid::from(record.value("uuid").toString()),
            .term          = record.value("term").toString(),
            .definition    = record.value("definition").toString(),
            .description   = record.value("description").toString(),
            .examples      = record.value("examples").toString(),
            .wikiUrl       = record.value("wikiUrl").toString(),
            .wikiImage     = record.value("wikiImage").toString(),
            .knowledgeArea = "",
            .groupUuid     = *gUuid,
            .lastEdit      = QDateTime::fromString(record.value("lastEdit").toString(), Qt::ISODate),
        };

        return info;
    }
};
