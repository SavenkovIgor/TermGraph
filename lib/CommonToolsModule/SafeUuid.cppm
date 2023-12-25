// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <string> // NOTE: this is need only due to weird ODR violation
#include <QString>
#include <QUuid>

export module CommonTools.SafeUuid;

export import CommonTools.HandyTypes;

export {

class SafeUuid : public QUuid
{
public:
    using List = std::vector<SafeUuid>;

    static Opt<SafeUuid> from(const QString& text)
    {
        auto uuid = QUuid(text);
        if (uuid.isNull())
            return std::nullopt;

        return SafeUuid(text);
    }

    static Opt<SafeUuid> from(const QUuid& uuid)
    {
        if (uuid.isNull())
            return std::nullopt;

        return SafeUuid(uuid.toString());
    }

    static SafeUuid generate() { return SafeUuid(QUuid::createUuid().toString()); }

    QUuid get() const { return QUuid(toString()); }

protected:
    explicit SafeUuid(const QString& text)
        : QUuid(text)
    {
        assert(!isNull());
    }

    bool isNull() const { return false; }
};

}
