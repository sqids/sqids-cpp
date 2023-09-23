#include <gtest/gtest.h>
#include <sqids/sqids.hpp>

TEST(Blocklist, IfNoCustomBlocklistParamUseTheDefaultBlocklist) {
    sqidscxx::Sqids<> sqids;

    EXPECT_EQ(sqids.decode("aho1e"), sqids.numbers({ 4572721 }));
    EXPECT_EQ(sqids.encode({ 4572721 }), "JExTR");
}

TEST(Blocklist, IfAnEmptyBlocklistParamPassedDontUseAnyBlocklist) {
    sqidscxx::Sqids<> sqids({ blocklist: {} });

    EXPECT_EQ(sqids.decode("aho1e"), sqids.numbers({ 4572721 }));
    EXPECT_EQ(sqids.encode({ 4572721 }), "aho1e");
}

TEST(Blocklist, IfANonEmptyBlocklistParamPassedUseOnlyThat) {
    sqidscxx::Sqids<> sqids({ blocklist: {
        "ArUO"  // originally encoded [100000]
    } });

    // Make sure we don't use the default blocklist
    EXPECT_EQ(sqids.decode("aho1e"), sqids.numbers({ 4572721 }));
    EXPECT_EQ(sqids.encode({ 4572721 }), "aho1e");

    // Make sure we are using the passed blocklist
    EXPECT_EQ(sqids.decode("ArUO"), sqids.numbers({ 100000 }));
    EXPECT_EQ(sqids.encode({ 100000 }), "QyG4");
    EXPECT_EQ(sqids.decode("QyG4"), sqids.numbers({ 100000 }));
}

TEST(Blocklist, Blocklist) {
    sqidscxx::Sqids<> sqids({ blocklist: {
        "JSwXFaosAN",  // Normal result of 1st encoding. Let's block that word on purpose
        "OCjV9JK64o",  // Result of 2nd encoding
        "rBHf",        // Result of 3rd encoding is `4rBHfOiqd3`. Let's block a substring
        "79SM",        // Result of 4th encoding is `dyhgw479SM`. Let's block the postfix
        "7tE6"         // Result of 4th encoding is `7tE6jdAHLe`. Let's block the prefix
    } });

    EXPECT_EQ(sqids.encode({ 1'000'000, 2'000'000 }), "1aYeB7bRUt");
    EXPECT_EQ(sqids.decode("1aYeB7bRUt"), sqids.numbers({ 1'000'000, 2'000'000 }));
}

TEST(Blocklist, DecodingBlocklistWordsShouldStillWork) {
    sqidscxx::Sqids<> sqids({ blocklist: { "86Rf07", "se8ojk", "ARsz1p", "Q8AI49", "5sQRZO" } });

    EXPECT_EQ(sqids.decode("86Rf07"), sqids.numbers({ 1, 2, 3 }));
    EXPECT_EQ(sqids.decode("se8ojk"), sqids.numbers({ 1, 2, 3 }));
    EXPECT_EQ(sqids.decode("ARsz1p"), sqids.numbers({ 1, 2, 3 }));
    EXPECT_EQ(sqids.decode("Q8AI49"), sqids.numbers({ 1, 2, 3 }));
    EXPECT_EQ(sqids.decode("5sQRZO"), sqids.numbers({ 1, 2, 3 }));
}

TEST(Blocklist, MatchAgainstAShortBlocklistWord) {
    sqidscxx::Sqids<> sqids({ blocklist: { "pnd" } });

    EXPECT_EQ(sqids.decode(sqids.encode({ 1000 })), sqids.numbers({ 1000 }));
}

TEST(Blocklist, BlocklistFilteringInConstructor) {
    sqidscxx::Sqids<> sqids({
        alphabet: "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        blocklist: { "sxnzkl" }  // Lowercase blocklist in uppercase-only alphabet
    });

    auto id = sqids.encode({ 1, 2, 3 });
    auto numbers = sqids.decode(id);

    EXPECT_EQ(id, "IBSHOZ");  // Without blocklist, would've been "SXNZKL"
    EXPECT_EQ(numbers, sqids.numbers({ 1, 2, 3 }));
}

TEST(Blocklist, MaxEncodingAttempts) {
    std::string alphabet = "abc";
    size_t minLength = 3;
    std::set<std::string> blocklist = { "cab", "abc", "bca" };

    sqidscxx::Sqids<> sqids({
        alphabet: alphabet,
        minLength: minLength,
        blocklist: blocklist
    });

    EXPECT_EQ(alphabet.size(), minLength);
    EXPECT_EQ(blocklist.size(), minLength);

    ASSERT_THROW(sqids.encode({ 0 }), std::runtime_error);
}
