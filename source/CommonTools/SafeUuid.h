// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QString>
#include <QUuid>

enum class UuidMode { Default, Url };

class SafeUuid : public QUuid
{
public:
    inline static std::optional<SafeUuid> from(const QString& text)
    {
        auto uuid = QUuid(text);
        if (uuid.isNull())
            return std::nullopt;

        return SafeUuid(text);
    }

    inline static std::optional<SafeUuid> from(const QUuid& uuid)
    {
        if (uuid.isNull())
            return std::nullopt;

        return SafeUuid(uuid.toString());
    }

    inline static SafeUuid generate() { return SafeUuid(QUuid::createUuid().toString()); }

    inline QUuid get() const { return QUuid(toString()); }

protected:
    explicit inline SafeUuid(const QString& text)
        : QUuid(text)
    {
        assert(!isNull());
    }

    inline bool isNull() const { return false; }
};
