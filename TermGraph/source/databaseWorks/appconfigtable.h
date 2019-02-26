#ifndef APPCONFIGTABLE_H
#define APPCONFIGTABLE_H

#include "./tblbase.h"
#include "./dbtablenames.h"

class AppConfigColumn
{
public:
    constexpr static auto parameter = TColumn("parameter", "TEXT UNIQUE");
    constexpr static auto value     = TColumn("value", "TEXT");

    constexpr static TColumn columns[] = {
        parameter,
        value
    };
};

class AppConfigTable : public TblBase
{
public:
    AppConfigTable(QSqlDatabase* base): TblBase(TableName::CONFIG, base) { }
    ~AppConfigTable() override = default;

    void initTable();

    bool isDbNeedUpdate();

protected:
    TColumn::List getAllColumns() const override;

private:
    constexpr static auto dbVersionPropertyName = "dbVersion";
    constexpr static auto dbVersion = 1;

    void insertVersionFieldIfNeed();
};

#endif // APPCONFIGTABLE_H
