// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <CommonTools/HandyTypes.h>
#include <CommonTools/Validator.h>

struct Point
{
    int x;
    int y;
};

enum class PointErrors { NotSmall, NotPositive };

class PointValidator : public Validator<Point, PointErrors>
{
public:
    static PointValidator smallPointValidator()
    {
        PointValidator ret;
        ret.addCheck(&PointValidator::isSmall, PointErrors::NotSmall);
        ret.addCheck(&PointValidator::isPositive, PointErrors::NotPositive);
        return ret;
    }

private:
    static bool isSmall(const Point& pt) { return pt.x < 100 && pt.y < 100; }
    static bool isPositive(const Point& pt) { return pt.x > 0 && pt.y > 0; }
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
    EXPECT_TRUE(errors1[0] == PointErrors::NotPositive);

    EXPECT_FALSE(v.check(badPt2));
    auto errors2 = v.lastErrors();
    ASSERT_EQ(errors2.size(), 2);
    EXPECT_TRUE(errors2[0] == PointErrors::NotSmall);
    EXPECT_TRUE(errors2[1] == PointErrors::NotPositive);
}
