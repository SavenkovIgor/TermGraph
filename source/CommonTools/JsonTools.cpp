// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringView>

export module CommonTools.JsonTools;

export class JsonTools
{
public:
    static QString prepareUuidParameter(QString param)
    {
        param.remove("%7B");
        param.remove("%7D");
        param.remove('{');
        param.remove('}');

        param.prepend('{');
        param.append('}');

        return param;
    }

    static QJsonObject updateKey(QJsonObject json, const QString& oldKey, const QString& newKey)
    {
        if (json.contains(oldKey) && !json.contains(newKey)) {
            json.insert(newKey, json[oldKey]);
        }

        if (json.contains(oldKey)) {
            json.remove(oldKey);
        }

        return json;
    }

    static QJsonObject addIfNotEmpty(QJsonObject json, const QString& key, const QJsonValue& value)
    {
        if (value.isString() && value.toString().isEmpty()) {
            return json;
        }

        if (value.isArray() && value.toArray().isEmpty()) {
            return json;
        }

        if (value.isObject() && value.toObject().isEmpty()) {
            return json;
        }

        json.insert(key, value);

        return json;
    }

    // Container keys
    constexpr static auto groupUuidsKey = "groupUuids";
    constexpr static auto groupsKey     = "groups";

    constexpr static auto termUuidsKey = "termUuids";
    constexpr static auto termsKey     = "terms";
    constexpr static auto oldTermsKey  = "nodesList";

    // Common keys
    constexpr static auto uuidKey     = "uuid";
    constexpr static auto lastEditKey = "lastEdit";

    // Group keys
    constexpr static auto nameKey          = "name";
    constexpr static auto commentKey       = "comment";
    constexpr static auto sizeKey          = "size";
    constexpr static auto nodesLastEditKey = "nodesLastEdit";

    // Term keys
    constexpr static auto termKey          = "term";
    constexpr static auto definitionKey    = "definition";
    constexpr static auto termDefKey       = "termDef";
    constexpr static auto descriptionKey   = "description";
    constexpr static auto examplesKey      = "examples";
    constexpr static auto wikiUrlKey       = "wikiUrl";
    constexpr static auto wikiImageKey     = "wikiImage";
    constexpr static auto knowledgeAreaKey = "area";
    constexpr static auto groupUuidKey     = "groupUuid";

    // Other stuff
    constexpr static auto termDefSeparator   = " - ";
    constexpr static auto termDefSeparatorSv = QStringView(u" - ");
};
