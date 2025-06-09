#include "source/helpers/wiki/WikiUrlChecker.h"
#include <gtest/gtest.h>
#include <QEventLoop>

TEST(WikiUrlCheckerTest, ExistingPage)
{
    QEventLoop loop;
    WikiUrlChecker checker;
    checker.check("Quark", [&](WikiUrlChecker::Result result) {
        ASSERT_TRUE(result.has_value()) << result.error();
        EXPECT_TRUE(result.value());
        loop.quit();
    });
    loop.exec();
}

TEST(WikiUrlCheckerTest, MissingPage)
{
    QEventLoop loop;
    WikiUrlChecker checker;
    checker.check("ThisPageShouldNotExist12345", [&](WikiUrlChecker::Result result) {
        ASSERT_TRUE(result.has_value()) << result.error();
        EXPECT_FALSE(result.value());
        loop.quit();
    });
    loop.exec();
}
