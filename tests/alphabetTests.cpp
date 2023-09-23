#include <gtest/gtest.h>
#include <sqids/sqids.hpp>

TEST(AlphabetTest, Simple) {
    sqidscxx::Sqids<> sqids({ alphabet: "0123456789abcdef" });

    const auto numbers(sqids.numbers({ 1, 2, 3 }));
    const auto id = "489158";

    EXPECT_EQ(sqids.encode(numbers), id);
    EXPECT_EQ(sqids.decode(id), numbers);
}

TEST(AlphabetTest, ShortAlphabet) {
    sqidscxx::Sqids<> sqids({ alphabet: "abc" });

    const auto numbers(sqids.numbers({ 1, 2, 3 }));

    EXPECT_EQ(sqids.decode(sqids.encode(numbers)), numbers);
}

TEST(AlphabetTest, LongAlphabet) {
    sqidscxx::Sqids<> sqids({ alphabet: "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_+|{}[];:'\"/?.>,<`~" });

    const auto numbers(sqids.numbers({ 1, 2, 3 }));

    EXPECT_EQ(sqids.decode(sqids.encode(numbers)), numbers);
}

TEST(AlphabetTest, MultibyteCharacters) {
    ASSERT_THROW(sqidscxx::Sqids<>({ alphabet: "ë1092" }), std::runtime_error);
}

TEST(AlphabetTest, RepeatingAlphabetCharacters) {
    ASSERT_THROW(sqidscxx::Sqids<>({ alphabet: "aabcdefg" }), std::runtime_error);
}

TEST(AlphabetTest, TooShortOfAnAlphabet) {
    ASSERT_THROW(sqidscxx::Sqids<>({ alphabet: "ab" }), std::runtime_error);
}
