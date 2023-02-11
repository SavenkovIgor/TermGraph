// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <functional>


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

    void clear() { mCheckList.clear(); }

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
