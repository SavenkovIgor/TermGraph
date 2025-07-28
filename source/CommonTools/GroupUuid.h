// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonObject>

#include "source/CommonTools/SafeUuid.h"

class GroupUuid final : public SafeUuid
{
public:
    static std::optional<GroupUuid> from(QString text, UuidMode mode = UuidMode::Default);
    static std::optional<GroupUuid> from(const QUuid& uuid);
    static GroupUuid                generate();

    class List : public std::vector<GroupUuid>
    {
    public:
        static std::optional<List> from(const QJsonObject& obj);
        static std::optional<List> from(const QByteArray& jsonBytes);

        explicit operator QJsonObject() const;
        explicit operator QByteArray() const;
    };

private:
    explicit GroupUuid(const QString& text);
};
