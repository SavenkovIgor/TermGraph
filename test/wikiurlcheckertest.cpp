#include "source/helpers/wiki/WikiUrlChecker.h"

#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QEventLoop>

TEST(WikiUrlCheckerTest, ExistingPage)
{
    auto             argc = 0;
    auto             argv = nullptr;
    QCoreApplication app(argc, argv);
    QEventLoop       loop;
    WikiUrlChecker   checker;
    checker.checkPageExist("Quark", [&](WikiUrlChecker::Result result) {
        ASSERT_TRUE(result.has_value()) << result.error();
        EXPECT_TRUE(result.value());
        loop.quit();
    });
    loop.exec();
}

TEST(WikiUrlCheckerTest, MissingPage)
{
    auto             argc = 0;
    auto             argv = nullptr;
    QCoreApplication app(argc, argv);
    QEventLoop       loop;
    WikiUrlChecker   checker;
    checker.checkPageExist("ThisPageShouldNotExist12345", [&](WikiUrlChecker::Result result) {
        ASSERT_TRUE(result.has_value()) << result.error();
        EXPECT_FALSE(result.value());
        loop.quit();
    });
    loop.exec();
}
