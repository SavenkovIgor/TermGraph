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

// TODO: split to GroupUuidEmpty, TermUuidEmpty, GroupUuidAlreadyExist, TermUuidAlreadyExist
// TODO: split to input errors, and exist data collision errors

enum class DbErrorCodes {
    UuidEmpty = 0,
    UuidAlreadyExist,
    UuidNotFound,

    GroupNameEmpty,
    GroupNameAlreadyExist,

    TermEmpty,
    TermNotFound,
    NewerTermVersionFound,
    ConnectionError
};

inline const char* error_message(int code)
{
    switch (static_cast<DbErrorCodes>(code)) {
    case DbErrorCodes::UuidEmpty: return "UuidEmpty";
    case DbErrorCodes::UuidAlreadyExist: return "UuidAlreadyExist";
    case DbErrorCodes::UuidNotFound: return "UuidNotExist";
    case DbErrorCodes::GroupNameEmpty: return "GroupNameEmpty";
    case DbErrorCodes::GroupNameAlreadyExist: return "GroupNameAlreadyExist";
    case DbErrorCodes::TermEmpty: return "TermEmpty";
    case DbErrorCodes::TermNotFound: return "TermNotFound";
    case DbErrorCodes::NewerTermVersionFound: return "NewerTermVersionFound";
    case DbErrorCodes::ConnectionError: return "ConnectionError";
    }
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
