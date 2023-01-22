// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <CommonTools/JsonTools.h>
#include <CommonTools/SafeUuid.h>

class GroupUuid final : public SafeUuid
{
public:
    inline static Opt<GroupUuid> create(QString text, UuidMode mode = UuidMode::Default)
    {
        if (mode == UuidMode::Url)
            text = JsonTools::prepareUuidParameter(text);

        if (auto safe = SafeUuid::create(text))
            return GroupUuid(text);

        return std::nullopt;
    }

    inline static Opt<GroupUuid> create(const QUuid& uuid)
    {
        if (auto safe = SafeUuid::create(uuid))
            return GroupUuid(uuid.toString());

        return std::nullopt;
    }

    inline static GroupUuid generate() { return GroupUuid(QUuid::createUuid().toString()); }

    class List : public std::vector<GroupUuid>
    {
    public:
        static inline Opt<List> create(const QJsonObject& obj)
        {
            List ret;

            if (!obj[JsonTools::groupUuidsKey].isArray())
                return std::nullopt;

            for (const auto& obj : obj[JsonTools::groupUuidsKey].toArray()) {
                if (auto uuid = GroupUuid::create(obj.toString())) {
                    ret.push_back(*uuid);
                } else {
                    qWarning("Wrong uuid in received data");
                }
            }

            return ret;
        }

        static inline Opt<List> create(const QByteArray& jsonBytes)
        {
            auto doc = QJsonDocument::fromJson(jsonBytes);

            if (doc.isNull())
                return std::nullopt;

            return create(doc.object());
        }

        explicit operator QJsonObject() const
        {
            QJsonArray arr;

            for (auto item : *this)
                arr.push_back(item.toString(StringFormat::WithoutBraces));

            QJsonObject obj;
            obj.insert(JsonTools::groupUuidsKey, arr);
            return obj;
        }

        explicit operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }
    };

private:
    explicit inline GroupUuid(const QString& text)
        : SafeUuid(text)
    {}
};
