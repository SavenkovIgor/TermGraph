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

#include <functional>

#include <CommonTools/SafeUuid.h>

template<typename Object, typename Errors>
class Validator
{
public:
    using Condition = std::function<bool(const Object&)>;
    using ErrorList = std::vector<Errors>;

    void addCheck(Condition condition, Errors error) { mCheckList.push_back({condition, error}); }

    bool check(const Object& obj)
    {
        bool ret = true;
        mLastErrors.clear();

        for (const auto& item : mCheckList) {
            if (!item.condition(obj)) {
                ret = false;
                mLastErrors.push_back(item.error);
            }
        }
        return ret;
    }

    ErrorList lastErrors() const { return mLastErrors; }

protected:
    // This class is only for nesting
    Validator() = default;

private:
    struct CheckItem
    {
        Condition   condition;
        Errors      error;
    };

    std::vector<CheckItem> mCheckList;
    ErrorList              mLastErrors;
};
