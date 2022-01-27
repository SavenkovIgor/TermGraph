/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <iostream>
#include <string>
#include <system_error>

enum class DbErrorCodes {
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
    NewerTermVersionFound,

    ConnectionError,
    JsonParseError,

    UnknownError
};

inline DbErrorCodes createDbError(std::string msg)
{
    // clang-format off
    if (msg == "GroupUuidInvalid")      return DbErrorCodes::GroupUuidInvalid;
    if (msg == "GroupUuidAlreadyExist") return DbErrorCodes::GroupUuidAlreadyExist;
    if (msg == "GroupUuidNotFound")     return DbErrorCodes::GroupUuidNotFound;
    if (msg == "GroupNameEmpty")        return DbErrorCodes::GroupNameEmpty;
    if (msg == "GroupNameAlreadyExist") return DbErrorCodes::GroupNameAlreadyExist;
    if (msg == "TermUuidInvalid")       return DbErrorCodes::TermUuidInvalid;
    if (msg == "TermUuidAlreadyExist")  return DbErrorCodes::TermUuidAlreadyExist;
    if (msg == "TermUuidNotFound")      return DbErrorCodes::TermUuidNotFound;
    if (msg == "TermEmpty")             return DbErrorCodes::TermEmpty;
    if (msg == "TermAlreadyExist")      return DbErrorCodes::TermAlreadyExist;
    if (msg == "TermNotFound")          return DbErrorCodes::TermNotFound;
    if (msg == "NewerTermVersionFound") return DbErrorCodes::NewerTermVersionFound;
    if (msg == "ConnectionError")       return DbErrorCodes::ConnectionError;
    if (msg == "JsonParseError")        return DbErrorCodes::JsonParseError;
    if (msg == "UnknownError")          return DbErrorCodes::UnknownError;
    // clang-format on

    return DbErrorCodes::UnknownError;
}

inline const char* error_message(int code)
{
    // clang-format off
    switch (static_cast<DbErrorCodes>(code)) {
    case DbErrorCodes::GroupUuidInvalid:      return "GroupUuidInvalid";
    case DbErrorCodes::GroupUuidAlreadyExist: return "GroupUuidAlreadyExist";
    case DbErrorCodes::GroupUuidNotFound:     return "GroupUuidNotFound";
    case DbErrorCodes::GroupNameEmpty:        return "GroupNameEmpty";
    case DbErrorCodes::GroupNameAlreadyExist: return "GroupNameAlreadyExist";

    case DbErrorCodes::TermUuidInvalid:       return "TermUuidInvalid";
    case DbErrorCodes::TermUuidAlreadyExist:  return "TermUuidAlreadyExist";
    case DbErrorCodes::TermUuidNotFound:      return "TermUuidNotFound";
    case DbErrorCodes::TermEmpty:             return "TermEmpty";
    case DbErrorCodes::TermAlreadyExist:      return "TermAlreadyExist";
    case DbErrorCodes::TermNotFound:          return "TermNotFound";
    case DbErrorCodes::NewerTermVersionFound: return "NewerTermVersionFound";

    case DbErrorCodes::ConnectionError:       return "ConnectionError";
    case DbErrorCodes::JsonParseError:        return "JsonParseError";
    case DbErrorCodes::UnknownError:          return "UnknownError";
    }
    // clang-format off

    return "UnreachableMsg";
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

inline std::error_code make_error_code(DbErrorCodes code)
{
    return {static_cast<int>(code), detail::DbErrorCodes_category::get()};
}

namespace std {
template<>
struct is_error_code_enum<DbErrorCodes> : true_type
{};
} // namespace std
