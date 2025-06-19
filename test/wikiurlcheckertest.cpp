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
    checker.checkPageExists("Quark", [&](auto result) {
        ASSERT_TRUE(result.has_value()) << result.error();
        EXPECT_TRUE(result.value());
        qInfo() << "Quark page exists:" << result.value();
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
    checker.checkPageExists("ThisPageShouldNotExist12345", [&](auto result) {
        ASSERT_TRUE(result.has_value()) << result.error();
        EXPECT_FALSE(result.value());
        qInfo() << "ThisPageShouldNotExist12345 page exists:" << result.value();
        loop.quit();
    });
    loop.exec();
}

TEST(WikiUrlCheckerTest, TenRequestsQueued)
{
    auto                 argc = 0;
    auto                 argv = nullptr;
    QCoreApplication     app(argc, argv);
    QEventLoop           loop;
    WikiUrlChecker       checker;
    std::vector<QString> terms = {"Dog", "NonexistentPage1234567890"};
    std::vector<bool>    called(terms.size(), false);
    int                  completed = 0;
    for (size_t i = 0; i < terms.size(); ++i) {
        checker.checkPageExists(terms[i], [i, &terms, &called, &completed, &loop](auto result) {
            called[i] = true;
            if (result.has_value()) {
                // Print for debug, but assert on value type
                SUCCEED() << "[" << i << "] '" << result.value() << "' exists: " << std::boolalpha << result.value();
            } else {
                ADD_FAILURE() << "[" << i << "] error: " << result.error();
            }
            qInfo() << terms[i] << "page exists:" << result.value();
            ++completed;
            if (completed == 10) {
                loop.quit();
            }
        });
    }
    QTimer::singleShot(15000, &loop, &QEventLoop::quit);
    loop.exec();
    for (size_t i = 0; i < called.size(); ++i) {
        EXPECT_TRUE(called[i]) << "Callback not called for request " << i;
    }
}
