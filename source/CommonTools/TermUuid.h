// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "source/CommonTools/HandyTypes.h"
#include "source/CommonTools/SafeUuid.h"

class TermUuid final : public SafeUuid
{
public:
    static std::optional<TermUuid> from(QString text, UuidMode mode = UuidMode::Default);
    static std::optional<TermUuid> from(const QUuid& uuid);
    static TermUuid                generate();

    class List : public std::vector<TermUuid>
    {
    public:
        static std::optional<List> from(const QJsonObject& obj);
        static std::optional<List> from(const QByteArray& jsonBytes);

        explicit operator QJsonObject() const;
        explicit operator QByteArray() const;
    };

private:
    explicit TermUuid(const QString& text);
};
