// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <string> // NOTE: this is need only due to weird ODR violation
#include <QString>
#include <QJsonObject>

export module CommonTools.GroupUuid;

import CommonTools.JsonTools;
import CommonTools.SafeUuid;

export {

class GroupUuid final : public SafeUuid
{
public:
    static Opt<GroupUuid> from(QString text, UuidMode mode = UuidMode::Default)
    {
        if (mode == UuidMode::Url)
            text = JsonTools::prepareUuidParameter(text);

        if (auto safe = SafeUuid::from(text))
            return GroupUuid(text);

        return std::nullopt;
    }

    static Opt<GroupUuid> from(const QUuid& uuid)
    {
        if (auto safe = SafeUuid::from(uuid))
            return GroupUuid(uuid.toString());

        return std::nullopt;
    }

    static GroupUuid generate() { return GroupUuid(QUuid::createUuid().toString()); }

    class List : public std::vector<GroupUuid>
    {
    public:
        static Opt<List> from(const QJsonObject& obj)
        {
            List ret;

            if (!obj[JsonTools::groupUuidsKey].isArray())
                return std::nullopt;

            for (const auto& obj : obj[JsonTools::groupUuidsKey].toArray()) {
                if (auto uuid = GroupUuid::from(obj.toString())) {
                    ret.push_back(*uuid);
                } else {
                    qWarning("Wrong uuid in received data");
                }
            }

            return ret;
        }

        static Opt<List> from(const QByteArray& jsonBytes)
        {
            auto doc = QJsonDocument::fromJson(jsonBytes);

            if (doc.isNull())
                return std::nullopt;

            return from(doc.object());
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
    explicit GroupUuid(const QString& text)
        : SafeUuid(text)
    {}
};

}
