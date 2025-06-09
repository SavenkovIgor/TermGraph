// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>

#include <QString>

enum ErrorCode {
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

inline const char* toChar(ErrorCode code)
{
    // clang-format off
    switch(code) {
        case ErrorCode::GroupUuidInvalid:                        return "GroupUuidInvalid";
        case ErrorCode::GroupUuidAlreadyExist:                   return "GroupUuidAlreadyExist";
        case ErrorCode::GroupUuidNotFound:                       return "GroupUuidNotFound";
        case ErrorCode::GroupNameEmpty:                          return "GroupNameEmpty";
        case ErrorCode::GroupNameAlreadyExist:                   return "GroupNameAlreadyExist";
        case ErrorCode::TermUuidInvalid:                         return "TermUuidInvalid";
        case ErrorCode::TermUuidAlreadyExist:                    return "TermUuidAlreadyExist";
        case ErrorCode::TermUuidNotFound:                        return "TermUuidNotFound";
        case ErrorCode::TermEmpty:                               return "TermEmpty";
        case ErrorCode::TermAlreadyExist:                        return "TermAlreadyExist";
        case ErrorCode::TermNotFound:                            return "TermNotFound";
        case ErrorCode::LastEditInvalid:                         return "LastEditInvalid";
        case ErrorCode::NewerTermVersionFound:                   return "NewerTermVersionFound";
        case ErrorCode::NodesLastEditInvalid:                    return "NodesLastEditInvalid";
        case ErrorCode::ConnectionError:                         return "ConnectionError";
        case ErrorCode::UnknownError:                            return "UnknownError";
        case ErrorCode::JsonParseError:                          return "JsonParseError";
        case ErrorCode::JsonNameFieldMissedOrWrongType:          return "JsonNameFieldMissedOrWrongType";
        case ErrorCode::JsonCommentFieldMissedOrWrongType:       return "JsonCommentFieldMissedOrWrongType";
        case ErrorCode::JsonSizeFieldMissedOrWrongType:          return "JsonSizeFieldMissedOrWrongType";
        case ErrorCode::JsonNodesFieldMissedOrWrongType:         return "JsonNodesFieldMissedOrWrongType";
        case ErrorCode::JsonNodesLastEditFieldMissedOrWrongType: return "JsonNodesLastEditFieldMissedOrWrongType";
        case ErrorCode::JsonUuidFieldMissedOrWrongType:          return "JsonUuidFieldMissedOrWrongType";
        case ErrorCode::JsonTermFieldMissedOrWrongType:          return "JsonTermFieldMissedOrWrongType";
        case ErrorCode::JsonDefinitionFieldMissedOrWrongType:    return "JsonDefinitionFieldMissedOrWrongType";
        case ErrorCode::JsonTermDefFieldWrongContentOrType:      return "JsonTermDefFieldWrongContentOrType";
        case ErrorCode::JsonDescriptionFieldMissedOrWrongType:   return "JsonDescriptionFieldMissedOrWrongType";
        case ErrorCode::JsonExamplesFieldMissedOrWrongType:      return "JsonExamplesFieldMissedOrWrongType";
        case ErrorCode::JsonWikiUrlFieldMissedOrWrongType:       return "JsonWikiUrlFieldMissedOrWrongType";
        case ErrorCode::JsonWikiImageFieldMissedOrWrongType:     return "JsonWikiImageFieldMissedOrWrongType";
        case ErrorCode::JsonGroupUuidFieldMissedOrWrongType:     return "JsonGroupUuidFieldMissedOrWrongType";
        case ErrorCode::JsonLastEditFieldMissedOrWrongType:      return "JsonLastEditFieldMissedOrWrongType";
    }
    // clang-format on

    return "UnknownError";
}

inline QString toQString(ErrorCode code) { return {toChar(code)}; }
inline QString toQString(int code) { return toQString(static_cast<ErrorCode>(code)); }

inline ErrorCode fromChar(const char* enumName)
{
    std::string msg(enumName);

    // clang-format off
    if (msg == "GroupUuidInvalid")                        return ErrorCode::GroupUuidInvalid;
    if (msg == "GroupUuidAlreadyExist")                   return ErrorCode::GroupUuidAlreadyExist;
    if (msg == "GroupUuidNotFound")                       return ErrorCode::GroupUuidNotFound;
    if (msg == "GroupNameEmpty")                          return ErrorCode::GroupNameEmpty;
    if (msg == "GroupNameAlreadyExist")                   return ErrorCode::GroupNameAlreadyExist;
    if (msg == "TermUuidInvalid")                         return ErrorCode::TermUuidInvalid;
    if (msg == "TermUuidAlreadyExist")                    return ErrorCode::TermUuidAlreadyExist;
    if (msg == "TermUuidNotFound")                        return ErrorCode::TermUuidNotFound;
    if (msg == "TermEmpty")                               return ErrorCode::TermEmpty;
    if (msg == "TermAlreadyExist")                        return ErrorCode::TermAlreadyExist;
    if (msg == "TermNotFound")                            return ErrorCode::TermNotFound;
    if (msg == "LastEditInvalid")                         return ErrorCode::LastEditInvalid;
    if (msg == "NewerTermVersionFound")                   return ErrorCode::NewerTermVersionFound;
    if (msg == "NodesLastEditInvalid")                    return ErrorCode::NodesLastEditInvalid;
    if (msg == "ConnectionError")                         return ErrorCode::ConnectionError;
    if (msg == "UnknownError")                            return ErrorCode::UnknownError;
    if (msg == "JsonParseError")                          return ErrorCode::JsonParseError;
    if (msg == "JsonNameFieldMissedOrWrongType")          return ErrorCode::JsonNameFieldMissedOrWrongType;
    if (msg == "JsonCommentFieldMissedOrWrongType")       return ErrorCode::JsonCommentFieldMissedOrWrongType;
    if (msg == "JsonSizeFieldMissedOrWrongType")          return ErrorCode::JsonSizeFieldMissedOrWrongType;
    if (msg == "JsonNodesFieldMissedOrWrongType")         return ErrorCode::JsonNodesFieldMissedOrWrongType;
    if (msg == "JsonNodesLastEditFieldMissedOrWrongType") return ErrorCode::JsonNodesLastEditFieldMissedOrWrongType;
    if (msg == "JsonUuidFieldMissedOrWrongType")          return ErrorCode::JsonUuidFieldMissedOrWrongType;
    if (msg == "JsonTermFieldMissedOrWrongType")          return ErrorCode::JsonTermFieldMissedOrWrongType;
    if (msg == "JsonDefinitionFieldMissedOrWrongType")    return ErrorCode::JsonDefinitionFieldMissedOrWrongType;
    if (msg == "JsonTermDefFieldWrongContentOrType")      return ErrorCode::JsonTermDefFieldWrongContentOrType;
    if (msg == "JsonDescriptionFieldMissedOrWrongType")   return ErrorCode::JsonDescriptionFieldMissedOrWrongType;
    if (msg == "JsonExamplesFieldMissedOrWrongType")      return ErrorCode::JsonExamplesFieldMissedOrWrongType;
    if (msg == "JsonWikiUrlFieldMissedOrWrongType")       return ErrorCode::JsonWikiUrlFieldMissedOrWrongType;
    if (msg == "JsonWikiImageFieldMissedOrWrongType")     return ErrorCode::JsonWikiImageFieldMissedOrWrongType;
    if (msg == "JsonGroupUuidFieldMissedOrWrongType")     return ErrorCode::JsonGroupUuidFieldMissedOrWrongType;
    if (msg == "JsonLastEditFieldMissedOrWrongType")      return ErrorCode::JsonLastEditFieldMissedOrWrongType;
    // clang-format on

    return ErrorCode::UnknownError;
}

inline ErrorCode fromQString(QString enumName)
{
    auto str = enumName.toStdString();
    return fromChar(str.c_str());
}

} // namespace Errors
