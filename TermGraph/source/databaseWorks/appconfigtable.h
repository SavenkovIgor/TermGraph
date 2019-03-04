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

    // TODO: Delete this later
    // This function must be called only once, when initing database
    void initTable() override;

    int getDbVersion();
    bool isDbVersionActual();
    void updateDbVersionNumber();

protected:
    TColumn::List getAllColumns() const override;

private:
    constexpr static auto dbVersionPropertyName = "dbVersion";
    constexpr static auto startDbVersion = 1;
    constexpr static auto dbVersion = 1;

    // Values works
    bool hasKey(const QString& key);

    void setValue(const QString& key, const QString& value);
    QString value(const QString& key, const QString& defaultValue);
};

#endif // APPCONFIGTABLE_H
