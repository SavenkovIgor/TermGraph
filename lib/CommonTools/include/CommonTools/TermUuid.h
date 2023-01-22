// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <CommonTools/JsonTools.h>
#include <CommonTools/SafeUuid.h>

class TermUuid final : public SafeUuid
{
public:
    inline static Opt<TermUuid> create(QString text, UuidMode mode = UuidMode::Default)
    {
        if (mode == UuidMode::Url)
            text = JsonTools::prepareUuidParameter(text);

        if (auto safe = SafeUuid::create(text))
            return TermUuid(text);

        return std::nullopt;
    }

    inline static Opt<TermUuid> create(const QUuid& uuid)
    {
        if (auto safe = SafeUuid::create(uuid))
            return TermUuid(uuid.toString());

        return std::nullopt;
    }

    inline static TermUuid generate() { return TermUuid(QUuid::createUuid().toString()); }

    class List : public std::vector<TermUuid>
    {
    public:
        static inline Opt<List> create(const QJsonObject& obj)
        {
            List ret;

            if (!obj[JsonTools::termUuidsKey].isArray())
                return std::nullopt;

            for (const auto& obj : obj[JsonTools::termUuidsKey].toArray()) {
                if (auto uuid = TermUuid::create(obj.toString())) {
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
            obj.insert(JsonTools::termUuidsKey, arr);
            return obj;
        }

        explicit operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }
    };

private:
    explicit inline TermUuid(const QString& text)
        : SafeUuid(text)
    {}
};
