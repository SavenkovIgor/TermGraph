// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QString>
#include <QStringView>

export module CommonTools.JsonKeys;

export namespace jsonKeys {

// Container keys
constexpr auto groupUuidsKey = "groupUuids";
constexpr auto groupsKey     = "groups";

constexpr auto termUuidsKey = "termUuids";
constexpr auto termsKey     = "terms";
constexpr auto oldTermsKey  = "nodesList";

// Common keys
constexpr auto uuidKey     = "uuid";
constexpr auto lastEditKey = "lastEdit";

// Group keys
constexpr auto nameKey          = "name";
constexpr auto commentKey       = "comment";
constexpr auto sizeKey          = "size";
constexpr auto nodesLastEditKey = "nodesLastEdit";

// Term keys
constexpr auto termKey          = "term";
constexpr auto definitionKey    = "definition";
constexpr auto termDefKey       = "termDef";
constexpr auto descriptionKey   = "description";
constexpr auto examplesKey      = "examples";
constexpr auto wikiUrlKey       = "wikiUrl";
constexpr auto wikiImageKey     = "wikiImage";
constexpr auto knowledgeAreaKey = "area";
constexpr auto groupUuidKey     = "groupUuid";

// Other stuff
constexpr auto termDefSeparator   = " - ";
constexpr auto termDefSeparatorSv = QStringView(u" - ");

} // namespace jsonKeys
