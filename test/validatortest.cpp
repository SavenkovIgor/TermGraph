// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <source/CommonTools/HandyTypes.h>
#include <source/CommonTools/Validator.h>

struct Point
{
    int x;
    int y;
};

class PointValidator : public Validator<Point>
{
public:
    static PointValidator smallPointValidator()
    {
        PointValidator ret;
        // Error codes are just for example
        ret.addCheck(&PointValidator::isSmall);
        ret.addCheck(&PointValidator::isPositive);
        return ret;
    }

private:
    static CheckResult isSmall(const Point& pt)
    {
        return checkOrError(pt.x < 100 && pt.y < 100, ErrorCodes::TermEmpty);
    }
    static CheckResult isPositive(const Point& pt)
    {
        return checkOrError(pt.x > 0 && pt.y > 0, ErrorCodes::GroupNameEmpty);
    }
};

TEST(ValidatorTest, Creation)
{
    Point goodPt{1, 1};
    Point badPt1{-1, 10};
    Point badPt2{1000, -1000};

    auto v = PointValidator::smallPointValidator();

    EXPECT_TRUE(v.check(goodPt));
    EXPECT_TRUE(v.lastErrors().empty());

    EXPECT_FALSE(v.check(badPt1));
    auto errors1 = v.lastErrors();
    ASSERT_EQ(errors1.size(), 1);
    EXPECT_TRUE(errors1[0] == ErrorCodes::GroupNameEmpty);

    EXPECT_FALSE(v.check(badPt2));
    auto errors2 = v.lastErrors();
    ASSERT_EQ(errors2.size(), 2);
    EXPECT_TRUE(errors2[0] == ErrorCodes::TermEmpty);
    EXPECT_TRUE(errors2[1] == ErrorCodes::GroupNameEmpty);
}
