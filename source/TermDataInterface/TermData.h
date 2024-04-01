// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <vector>

#include <QByteArray>
#include <QDateTime>
#include <QJsonObject>
#include <QString>
#include <QUuid>

#include "source/CommonTools/GroupUuid.h"
#include "source/CommonTools/HandyTypes.h"
#include "source/CommonTools/JsonTools.h"
#include "source/CommonTools/TermUuid.h"

#include "TermValidator.h"

// TODO: Check tests!
// TODO: Make class and make fields private
struct TermData
{
    Opt<TermUuid> uuid;
    QString       term;
    QString       definition;
    QString       description;
    QString       examples;
    QString       wikiUrl;
    QString       wikiImage;
    QString       knowledgeArea;
    GroupUuid     groupUuid;
    QDateTime     lastEdit;

    inline bool isNull() const { return uuid.has_value() && term.isEmpty(); }

    inline bool operator==(const TermData& rhs) const = default;

    enum class JsonCheckMode { Import, AddTerm, UpdateTerm, Minimal };

    static TermData createGhost(const QString& term, GroupUuid groupUuid);

    // --- JSON ---
    static Opt<TermData> from(const QJsonObject& obj, JsonCheckMode mode);
    static Opt<TermData> from(const QByteArray& jsonBytes, JsonCheckMode mode);

    QJsonObject toQJsonObject() const;
    QJsonObject toMinimalQJsonObject() const;
    QByteArray  toQByteArray() const;

    explicit operator QJsonObject() const { return toQJsonObject(); }
    explicit operator QByteArray() const { return toQByteArray(); }

    class List : public std::vector<TermData>
    {
    public:
        static Opt<List> from(const QJsonObject& obj);
        static List      from(const QJsonArray& json, JsonCheckMode mode = JsonCheckMode::Import);
        static Opt<List> from(const QByteArray& jsonBytes);

        QJsonObject toQJsonObject() const;
        QJsonArray  toQJsonArray() const;
        QByteArray  toQByteArray() const;

        explicit operator QJsonObject() const { return toQJsonObject(); }
        explicit operator QJsonArray() const { return toQJsonArray(); }
        explicit operator QByteArray() const { return toQByteArray(); }
    };
};
