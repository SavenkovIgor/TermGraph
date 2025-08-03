// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QJsonArray>
#include <QJsonObject>

export module CommonTools.JsonTools;

export namespace jsonTools {

QString prepareUuidParameter(QString param)
{
    param.remove("%7B");
    param.remove("%7D");
    param.remove('{');
    param.remove('}');

    param.prepend('{');
    param.append('}');

    return param;
}

QJsonObject updateKey(QJsonObject json, const QString& oldKey, const QString& newKey)
{
    if (json.contains(oldKey) && !json.contains(newKey)) {
        json.insert(newKey, json[oldKey]);
    }

    if (json.contains(oldKey)) {
        json.remove(oldKey);
    }

    return json;
}

QJsonObject addIfNotEmpty(QJsonObject json, const QString& key, const QJsonValue& value)
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

} // namespace jsonTools
