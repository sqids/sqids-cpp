#include <gtest/gtest.h>
#include <sqids/sqids.hpp>

#define UPPER 1'000'000

TEST(Uniques, WithPadding) {
    sqidscxx::Sqids<> sqids({ minLength: sqidscxx::SqidsOptions().alphabet.size() });
    std::set<std::string> set;

    for (uint64_t i = 0; i != UPPER; i++) {
        const auto numbers = sqids.numbers({ i });
        const auto id = sqids.encode(numbers);
        set.insert(id);

        EXPECT_EQ(sqids.decode(id), numbers);
    }

    EXPECT_EQ(set.size(), UPPER);
}

TEST(Uniques, LowRanges) {
    sqidscxx::Sqids<> sqids;
    std::set<std::string> set;

    for (uint64_t i = 0; i != UPPER; i++) {
        const auto numbers = sqids.numbers({ i });
        const auto id = sqids.encode(numbers);
        set.insert(id);

        EXPECT_EQ(sqids.decode(id), numbers);
    }

    EXPECT_EQ(set.size(), UPPER);
}

TEST(Uniques, HighRanges) {
    sqidscxx::Sqids<> sqids;
    std::set<std::string> set;

    for (uint64_t i = 100'000'000; i != 100'000'000 + UPPER; i++) {
        const auto numbers = sqids.numbers({ i });
        const auto id = sqids.encode(numbers);
        set.insert(id);

        EXPECT_EQ(sqids.decode(id), numbers);
    }

    EXPECT_EQ(set.size(), UPPER);
}

TEST(Uniques, Multi) {
    sqidscxx::Sqids<> sqids;
    std::set<std::string> set;

    for (uint64_t i = 0; i != UPPER; i++) {
        const auto numbers = sqids.numbers({ i, i, i, i, i });
        const auto id = sqids.encode(numbers);
        set.insert(id);

        EXPECT_EQ(sqids.decode(id), numbers);
    }

    EXPECT_EQ(set.size(), UPPER);
}
