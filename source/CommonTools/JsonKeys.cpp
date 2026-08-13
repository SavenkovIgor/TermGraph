// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QString>
#include <QStringView>

export module CommonTools.JsonKeys;

using namespace Qt::StringLiterals;

export namespace jsonKeys {

// Container keys
constexpr auto groupUuidsKey = QStringView(u"groupUuids");
constexpr auto groupsKey     = QStringView(u"groups");

constexpr auto termUuidsKey = QStringView(u"termUuids");
constexpr auto termsKey     = QStringView(u"terms");
constexpr auto oldTermsKey  = QStringView(u"nodesList");

// Common keys
constexpr auto uuidKey     = QStringView(u"uuid");
constexpr auto lastEditKey = QStringView(u"lastEdit");

// Group keys
constexpr auto nameKey          = QStringView(u"name");
constexpr auto commentKey       = QStringView(u"comment");
constexpr auto sizeKey          = QStringView(u"size");
constexpr auto nodesLastEditKey = QStringView(u"nodesLastEdit");

// Term keys
constexpr auto termKey          = QStringView(u"term");
constexpr auto definitionKey    = QStringView(u"definition");
constexpr auto termDefKey       = QStringView(u"termDef");
constexpr auto descriptionKey   = QStringView(u"description");
constexpr auto examplesKey      = QStringView(u"examples");
constexpr auto wikiUrlKey       = QStringView(u"wikiUrl");
constexpr auto wikiImageKey     = QStringView(u"wikiImage");
constexpr auto knowledgeAreaKey = QStringView(u"area");
constexpr auto groupUuidKey     = QStringView(u"groupUuid");

// Other stuff
constexpr auto termDefSeparator   = QStringView(u" - ");
constexpr auto termDefSeparatorSv = QStringView(u" - ");

} // namespace jsonKeys
