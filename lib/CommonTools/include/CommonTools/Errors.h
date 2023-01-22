// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <iostream>
#include <string>
#include <system_error>

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
    JsonDescriptionFieldMissedOrWrongType,
    JsonExamplesFieldMissedOrWrongType,
    JsonWikiUrlFieldMissedOrWrongType,
    JsonWikiImageFieldMissedOrWrongType,

    JsonGroupUuidFieldMissedOrWrongType,

    JsonLastEditFieldMissedOrWrongType,
};

namespace Errors {

inline const char* toChar(ErrorCodes code) {
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

inline ErrorCodes fromChar(const char* enumName) {
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
    if (msg == "JsonDescriptionFieldMissedOrWrongType")   return ErrorCodes::JsonDescriptionFieldMissedOrWrongType;
    if (msg == "JsonExamplesFieldMissedOrWrongType")      return ErrorCodes::JsonExamplesFieldMissedOrWrongType;
    if (msg == "JsonWikiUrlFieldMissedOrWrongType")       return ErrorCodes::JsonWikiUrlFieldMissedOrWrongType;
    if (msg == "JsonWikiImageFieldMissedOrWrongType")     return ErrorCodes::JsonWikiImageFieldMissedOrWrongType;
    if (msg == "JsonGroupUuidFieldMissedOrWrongType")     return ErrorCodes::JsonGroupUuidFieldMissedOrWrongType;
    if (msg == "JsonLastEditFieldMissedOrWrongType")      return ErrorCodes::JsonLastEditFieldMissedOrWrongType;
    // clang-format on

    return ErrorCodes::UnknownError;
}

inline ErrorCodes fromQString(QString enumName) {
    auto str = enumName.toStdString();
    return fromChar(str.c_str());
}

}

inline const char* error_message(int code)
{
    return Errors::toChar(static_cast<ErrorCodes>(code));
}

namespace detail {
class DbErrorCodes_category : public std::error_category
{
public:
    DbErrorCodes_category() {}

    const char* name() const noexcept final { return "DataError"; }

    std::string message(int code) const final { return error_message(code); }

    const static error_category& get()
    {
        const static DbErrorCodes_category c;
        return c;
    }
};
} // namespace detail

extern inline const detail::DbErrorCodes_category& DbErrorCodes_category()
{
    static detail::DbErrorCodes_category c;
    return c;
}

inline std::error_code make_error_code(ErrorCodes code)
{
    return {static_cast<int>(code), detail::DbErrorCodes_category::get()};
}

namespace std {
template<>
struct is_error_code_enum<ErrorCodes> : true_type
{};
} // namespace std