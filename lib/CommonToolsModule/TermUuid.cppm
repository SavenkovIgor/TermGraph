// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <string> // NOTE: this is need only due to weird ODR violation
#include <QString>
#include <QJsonObject>

export module CommonTools.TermUuid;

import CommonTools.JsonTools;
import CommonTools.SafeUuid;

export {

class TermUuid final : public SafeUuid
{
public:
    static Opt<TermUuid> from(QString text, UuidMode mode = UuidMode::Default)
    {
        if (mode == UuidMode::Url)
            text = JsonTools::prepareUuidParameter(text);

        if (auto safe = SafeUuid::from(text))
            return TermUuid(text);

        return std::nullopt;
    }

    static Opt<TermUuid> from(const QUuid& uuid)
    {
        if (auto safe = SafeUuid::from(uuid))
            return TermUuid(uuid.toString());

        return std::nullopt;
    }

    static TermUuid generate() { return TermUuid(QUuid::createUuid().toString()); }

    class List : public std::vector<TermUuid>
    {
    public:
        static Opt<List> from(const QJsonObject& obj)
        {
            List ret;

            if (!obj[JsonTools::termUuidsKey].isArray())
                return std::nullopt;

            for (const auto& obj : obj[JsonTools::termUuidsKey].toArray()) {
                if (auto uuid = TermUuid::from(obj.toString())) {
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
            obj.insert(JsonTools::termUuidsKey, arr);
            return obj;
        }

        explicit operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }
    };

private:
    explicit TermUuid(const QString& text)
        : SafeUuid(text)
    {}
};

}
