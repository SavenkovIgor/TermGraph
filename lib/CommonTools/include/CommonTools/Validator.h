// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <functional>

#include <CommonTools/Errors.h>
#include <CommonTools/HandyTypes.h>


template<typename Object>
class Validator
{
public:
    using Condition = std::function<bool(const Object&)>;
    using ErrorList = std::vector<ErrorCodes>;

    void addCheck(Condition condition, ErrorCodes error) { mCheckList.push_back({condition, error}); }

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
        ErrorCodes      error;
    };

    std::vector<CheckItem> mCheckList;
    ErrorList              mLastErrors;
};
