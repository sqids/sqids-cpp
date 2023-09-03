#include <gtest/gtest.h>
#include <sqids/sqids.hpp>

TEST(Blocklist, IfNoCustomBlocklistParamUseTheDefaultBlocklist) {
    sqidscxx::Sqids<> sqids;

    EXPECT_EQ(sqids.decode("sexy"), sqids.numbers({ 200044 }));
    EXPECT_EQ(sqids.encode({ 200044 }), "d171vI");
}

TEST(Blocklist, IfAnEmptyBlocklistParamPassedDontUseAnyBlocklist) {
    sqidscxx::Sqids<> sqids({ blocklist: {} });

    EXPECT_EQ(sqids.decode("sexy"), sqids.numbers({ 200044 }));
    EXPECT_EQ(sqids.encode({ 200044 }), "sexy");
}

TEST(Blocklist, IfANonEmptyBlocklistParamPassedUseOnlyThat) {
    sqidscxx::Sqids<> sqids({ blocklist: { 
        "AvTg"  // originally encoded [100000]
    } });

    // Make sure we don't use the default blocklist
    EXPECT_EQ(sqids.decode("sexy"), sqids.numbers({ 200044 }));
    EXPECT_EQ(sqids.encode({ 200044 }), "sexy");

    // Make sure we are using the passed blocklist
    EXPECT_EQ(sqids.decode("AvTg"), sqids.numbers({ 100000 }));
    EXPECT_EQ(sqids.encode({ 100000 }), "7T1X8k");
    EXPECT_EQ(sqids.decode("7T1X8k"), sqids.numbers({ 100000 }));
}

TEST(Blocklist, Blocklist) {
    sqidscxx::Sqids<> sqids({ blocklist: { 
        "8QRLaD",    // Normal result of 1st encoding -- let's block that word on purpose
        "7T1cd0dL",  // Result of 2nd encoding
        "UeIe",      // Result of 3rd encoding is `RA8UeIe7` -- let's block a substring
        "imhw",      // Result of 4th encoding is `WM3Limhw` -- let's block the postfix
        "LfUQ"       // Result of 4th encoding is `LfUQh4HN` -- let's block the prefix
    } });

    EXPECT_EQ(sqids.encode({ 1, 2, 3}), "TM0x1Mxz");
    EXPECT_EQ(sqids.decode("TM0x1Mxz"), sqids.numbers({ 1, 2, 3 }));
}

TEST(Blocklist, DecodingBlocklistWordsShouldStillWork) {
    sqidscxx::Sqids<> sqids({ blocklist: { "8QRLaD", "7T1cd0dL", "RA8UeIe7", "WM3Limhw", "LfUQh4HN" } });

    EXPECT_EQ(sqids.decode("8QRLaD"), sqids.numbers({ 1, 2, 3 }));
    EXPECT_EQ(sqids.decode("7T1cd0dL"), sqids.numbers({ 1, 2, 3 }));
    EXPECT_EQ(sqids.decode("RA8UeIe7"), sqids.numbers({ 1, 2, 3 }));
    EXPECT_EQ(sqids.decode("WM3Limhw"), sqids.numbers({ 1, 2, 3 }));
    EXPECT_EQ(sqids.decode("LfUQh4HN"), sqids.numbers({ 1, 2, 3 }));
}

TEST(Blocklist, MatchAgainstAShortBlocklistWord) {
    sqidscxx::Sqids<> sqids({ blocklist: { "pPQ" } });

    EXPECT_EQ(sqids.decode(sqids.encode({ 1000 })), sqids.numbers({ 1000 }));
}

TEST(Blocklist, BlocklistFilteringInConstructor) {
    sqidscxx::Sqids<> sqids({ 
        alphabet: "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        blocklist: { "sqnmpn" }  // Lowercase blocklist in uppercase-only alphabet
    });

    auto id = sqids.encode({ 1, 2, 3 });
    auto numbers = sqids.decode(id);

    EXPECT_EQ(id, "ULPBZGBM");  // Without blocklist, would've been "SQNMPN"
    EXPECT_EQ(numbers, sqids.numbers({ 1, 2, 3 }));
}
