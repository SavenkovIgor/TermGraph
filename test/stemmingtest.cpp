#include <gtest/gtest.h>
#include "source/helpers/stemming/Stemmer.h"

TEST(Stemmer, Russian)
{
    auto st = create_stemmer(StemmingLanguage::Russian);
    EXPECT_EQ(st->stem(QString::fromUtf8("белое")), QString::fromUtf8("бел"));
    EXPECT_EQ(st->stem(QString::fromUtf8("ягод")), QString::fromUtf8("ягод"));
}

TEST(Stemmer, English)
{
    auto st = create_stemmer(StemmingLanguage::English);
    EXPECT_EQ(st->stem("playing"), "play");
    EXPECT_EQ(st->stem("connected"), "connect");
}
