// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <expected>
#include <functional>

#include "source/CommonTools/Errors.h"

export module CommonTools.Validator;

using namespace std;

export template<typename Object>
class Validator
{
public:
    // if return value is empty, then check is passed
    // else check is failed and error code is returned
    using CheckResult = expected<void, ErrorCode>;
    using Condition   = function<CheckResult(const Object&)>;
    using ErrorList   = vector<ErrorCode>;

    void addCheck(Condition condition) { mCheckList.push_back(condition); }

    bool check(const Object& obj)
    {
        bool ret = true;
        mLastErrors.clear();

        for (const auto& checkCondition : mCheckList) {
            if (auto res = checkCondition(obj); !res.has_value()) {
                ret = false;
                mLastErrors.push_back(res.error());
            }
        }
        return ret;
    }

    ErrorList lastErrors() const { return mLastErrors; }

    static CheckResult checkOrError(bool condition, ErrorCode error)
    {
        return condition ? expected<void, ErrorCode>{} : unexpected(error);
    }

    void clear() { mCheckList.clear(); }

protected:
    // This class is only for nesting
    Validator() = default;

private:
    vector<Condition> mCheckList;
    ErrorList         mLastErrors;
};
