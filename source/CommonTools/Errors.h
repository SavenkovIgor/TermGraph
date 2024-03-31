// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>

#include <QString>

enum ErrorCodes {
    GroupUuidInvalid = 0,
    GroupUuidAlreadyExist,
    GroupUuidNotFound,

    GroupNameEmpty,
    GroupNameAlreadyExist,

    TermUuidInvalid,
    TermUuidAlreadyExist,
    TermUuidNotFound,

    TermEmpty,
    TermAlreadyExist,
    TermNotFound,

    LastEditInvalid,
    NewerTermVersionFound,
    NodesLastEditInvalid,

    ConnectionError,

    UnknownError,

    // Json errors
    JsonParseError,

    // Group errors
    JsonNameFieldMissedOrWrongType,
    JsonCommentFieldMissedOrWrongType,
    JsonSizeFieldMissedOrWrongType,

    JsonNodesFieldMissedOrWrongType,
    JsonNodesLastEditFieldMissedOrWrongType,

    // Term errors
    JsonUuidFieldMissedOrWrongType,
    JsonTermFieldMissedOrWrongType,

    JsonDefinitionFieldMissedOrWrongType,
    JsonTermDefFieldWrongContentOrType,
    JsonDescriptionFieldMissedOrWrongType,
    JsonExamplesFieldMissedOrWrongType,
    JsonWikiUrlFieldMissedOrWrongType,
    JsonWikiImageFieldMissedOrWrongType,

    JsonGroupUuidFieldMissedOrWrongType,

    JsonLastEditFieldMissedOrWrongType,
};

namespace Errors {

inline const char* toChar(ErrorCodes code)
{
    // clang-format off
    switch(code) {
        case ErrorCodes::GroupUuidInvalid:                        return "GroupUuidInvalid";
        case ErrorCodes::GroupUuidAlreadyExist:                   return "GroupUuidAlreadyExist";
        case ErrorCodes::GroupUuidNotFound:                       return "GroupUuidNotFound";
        case ErrorCodes::GroupNameEmpty:                          return "GroupNameEmpty";
        case ErrorCodes::GroupNameAlreadyExist:                   return "GroupNameAlreadyExist";
        case ErrorCodes::TermUuidInvalid:                         return "TermUuidInvalid";
        case ErrorCodes::TermUuidAlreadyExist:                    return "TermUuidAlreadyExist";
        case ErrorCodes::TermUuidNotFound:                        return "TermUuidNotFound";
        case ErrorCodes::TermEmpty:                               return "TermEmpty";
        case ErrorCodes::TermAlreadyExist:                        return "TermAlreadyExist";
        case ErrorCodes::TermNotFound:                            return "TermNotFound";
        case ErrorCodes::LastEditInvalid:                         return "LastEditInvalid";
        case ErrorCodes::NewerTermVersionFound:                   return "NewerTermVersionFound";
        case ErrorCodes::NodesLastEditInvalid:                    return "NodesLastEditInvalid";
        case ErrorCodes::ConnectionError:                         return "ConnectionError";
        case ErrorCodes::UnknownError:                            return "UnknownError";
        case ErrorCodes::JsonParseError:                          return "JsonParseError";
        case ErrorCodes::JsonNameFieldMissedOrWrongType:          return "JsonNameFieldMissedOrWrongType";
        case ErrorCodes::JsonCommentFieldMissedOrWrongType:       return "JsonCommentFieldMissedOrWrongType";
        case ErrorCodes::JsonSizeFieldMissedOrWrongType:          return "JsonSizeFieldMissedOrWrongType";
        case ErrorCodes::JsonNodesFieldMissedOrWrongType:         return "JsonNodesFieldMissedOrWrongType";
        case ErrorCodes::JsonNodesLastEditFieldMissedOrWrongType: return "JsonNodesLastEditFieldMissedOrWrongType";
        case ErrorCodes::JsonUuidFieldMissedOrWrongType:          return "JsonUuidFieldMissedOrWrongType";
        case ErrorCodes::JsonTermFieldMissedOrWrongType:          return "JsonTermFieldMissedOrWrongType";
        case ErrorCodes::JsonDefinitionFieldMissedOrWrongType:    return "JsonDefinitionFieldMissedOrWrongType";
        case ErrorCodes::JsonTermDefFieldWrongContentOrType:      return "JsonTermDefFieldWrongContentOrType";
        case ErrorCodes::JsonDescriptionFieldMissedOrWrongType:   return "JsonDescriptionFieldMissedOrWrongType";
        case ErrorCodes::JsonExamplesFieldMissedOrWrongType:      return "JsonExamplesFieldMissedOrWrongType";
        case ErrorCodes::JsonWikiUrlFieldMissedOrWrongType:       return "JsonWikiUrlFieldMissedOrWrongType";
        case ErrorCodes::JsonWikiImageFieldMissedOrWrongType:     return "JsonWikiImageFieldMissedOrWrongType";
        case ErrorCodes::JsonGroupUuidFieldMissedOrWrongType:     return "JsonGroupUuidFieldMissedOrWrongType";
        case ErrorCodes::JsonLastEditFieldMissedOrWrongType:      return "JsonLastEditFieldMissedOrWrongType";
    }
    // clang-format on

    return "UnknownError";
}

inline QString toQString(ErrorCodes code) { return {toChar(code)}; }
inline QString toQString(int code) { return toQString(static_cast<ErrorCodes>(code)); }

inline ErrorCodes fromChar(const char* enumName)
{
    std::string msg(enumName);

    // clang-format off
    if (msg == "GroupUuidInvalid")                        return ErrorCodes::GroupUuidInvalid;
    if (msg == "GroupUuidAlreadyExist")                   return ErrorCodes::GroupUuidAlreadyExist;
    if (msg == "GroupUuidNotFound")                       return ErrorCodes::GroupUuidNotFound;
    if (msg == "GroupNameEmpty")                          return ErrorCodes::GroupNameEmpty;
    if (msg == "GroupNameAlreadyExist")                   return ErrorCodes::GroupNameAlreadyExist;
    if (msg == "TermUuidInvalid")                         return ErrorCodes::TermUuidInvalid;
    if (msg == "TermUuidAlreadyExist")                    return ErrorCodes::TermUuidAlreadyExist;
    if (msg == "TermUuidNotFound")                        return ErrorCodes::TermUuidNotFound;
    if (msg == "TermEmpty")                               return ErrorCodes::TermEmpty;
    if (msg == "TermAlreadyExist")                        return ErrorCodes::TermAlreadyExist;
    if (msg == "TermNotFound")                            return ErrorCodes::TermNotFound;
    if (msg == "LastEditInvalid")                         return ErrorCodes::LastEditInvalid;
    if (msg == "NewerTermVersionFound")                   return ErrorCodes::NewerTermVersionFound;
    if (msg == "NodesLastEditInvalid")                    return ErrorCodes::NodesLastEditInvalid;
    if (msg == "ConnectionError")                         return ErrorCodes::ConnectionError;
    if (msg == "UnknownError")                            return ErrorCodes::UnknownError;
    if (msg == "JsonParseError")                          return ErrorCodes::JsonParseError;
    if (msg == "JsonNameFieldMissedOrWrongType")          return ErrorCodes::JsonNameFieldMissedOrWrongType;
    if (msg == "JsonCommentFieldMissedOrWrongType")       return ErrorCodes::JsonCommentFieldMissedOrWrongType;
    if (msg == "JsonSizeFieldMissedOrWrongType")          return ErrorCodes::JsonSizeFieldMissedOrWrongType;
    if (msg == "JsonNodesFieldMissedOrWrongType")         return ErrorCodes::JsonNodesFieldMissedOrWrongType;
    if (msg == "JsonNodesLastEditFieldMissedOrWrongType") return ErrorCodes::JsonNodesLastEditFieldMissedOrWrongType;
    if (msg == "JsonUuidFieldMissedOrWrongType")          return ErrorCodes::JsonUuidFieldMissedOrWrongType;
    if (msg == "JsonTermFieldMissedOrWrongType")          return ErrorCodes::JsonTermFieldMissedOrWrongType;
    if (msg == "JsonDefinitionFieldMissedOrWrongType")    return ErrorCodes::JsonDefinitionFieldMissedOrWrongType;
    if (msg == "JsonTermDefFieldWrongContentOrType")      return ErrorCodes::JsonTermDefFieldWrongContentOrType;
    if (msg == "JsonDescriptionFieldMissedOrWrongType")   return ErrorCodes::JsonDescriptionFieldMissedOrWrongType;
    if (msg == "JsonExamplesFieldMissedOrWrongType")      return ErrorCodes::JsonExamplesFieldMissedOrWrongType;
    if (msg == "JsonWikiUrlFieldMissedOrWrongType")       return ErrorCodes::JsonWikiUrlFieldMissedOrWrongType;
    if (msg == "JsonWikiImageFieldMissedOrWrongType")     return ErrorCodes::JsonWikiImageFieldMissedOrWrongType;
    if (msg == "JsonGroupUuidFieldMissedOrWrongType")     return ErrorCodes::JsonGroupUuidFieldMissedOrWrongType;
    if (msg == "JsonLastEditFieldMissedOrWrongType")      return ErrorCodes::JsonLastEditFieldMissedOrWrongType;
    // clang-format on

    return ErrorCodes::UnknownError;
}

inline ErrorCodes fromQString(QString enumName)
{
    auto str = enumName.toStdString();
    return fromChar(str.c_str());
}

} // namespace Errors
