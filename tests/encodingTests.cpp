#include <gtest/gtest.h>
#include <sqids/sqids.hpp>

TEST(Encoding, Simple) {
    sqidscxx::Sqids<> sqids;

    const auto numbers(sqids.numbers({ 1, 2, 3 }));
    const auto id = "8QRLaD";

    EXPECT_EQ(sqids.encode(numbers), id);
    EXPECT_EQ(sqids.decode(id), numbers);
}

TEST(Encoding, DifferentInputs) {
    sqidscxx::Sqids<> sqids;

    const auto numbers(sqids.numbers({ 0, 0, 0, 1, 2, 3, 100, 1'000, 100'000, 1'000'000, sqids.maxValue }));

    EXPECT_EQ(sqids.decode(sqids.encode(numbers)), numbers);
}

typedef std::vector<std::pair<std::string, std::vector<uint64_t>>> IdAndNumbersPairs;

TEST(Encoding, IncrementalNumbers) {
    sqidscxx::Sqids<> sqids;

    const IdAndNumbersPairs pairs = {
        { "bV", { 0 } },
	{ "U9", { 1 } },
	{ "g8", { 2 } },
	{ "Ez", { 3 } },
	{ "V8", { 4 } },
	{ "ul", { 5 } },
	{ "O3", { 6 } },
	{ "AF", { 7 } },
	{ "ph", { 8 } },
	{ "n8", { 9 } }
    };

    for (auto pair : pairs) {
        const auto id = pair.first;
        const auto numbers = pair.second;

        EXPECT_EQ(sqids.encode(numbers), id);
        EXPECT_EQ(sqids.decode(id), numbers);
    }
}

TEST(Encoding, IncrementalNumbersSameIndex0) {
    sqidscxx::Sqids<> sqids;

    const IdAndNumbersPairs pairs = {
        { "SrIu", { 0, 0 } },
	{ "nZqE", { 0, 1 } },
	{ "tJyf", { 0, 2 } },
	{ "e86S", { 0, 3 } },
	{ "rtC7", { 0, 4 } },
	{ "sQ8R", { 0, 5 } },
	{ "uz2n", { 0, 6 } },
	{ "7Td9", { 0, 7 } },
	{ "3nWE", { 0, 8 } },
	{ "mIxM", { 0, 9 } }
    };

    for (auto pair : pairs) {
        const auto id = pair.first;
        const auto numbers = pair.second;

        EXPECT_EQ(sqids.encode(numbers), id);
        EXPECT_EQ(sqids.decode(id), numbers);
    }
}

TEST(Encoding, IncrementalNumbersSameIndex1) {
    sqidscxx::Sqids<> sqids;

    const IdAndNumbersPairs pairs = {
        { "SrIu", { 0, 0 } },
	{ "nbqh", { 1, 0 } },
	{ "t4yj", { 2, 0 } },
	{ "eQ6L", { 3, 0 } },
	{ "r4Cc", { 4, 0 } },
	{ "sL82", { 5, 0 } },
	{ "uo2f", { 6, 0 } },
	{ "7Zdq", { 7, 0 } },
	{ "36Wf", { 8, 0 } },
	{ "m4xT", { 9, 0 } }
    };

    for (auto pair : pairs) {
        const auto id = pair.first;
        const auto numbers = pair.second;

        EXPECT_EQ(sqids.encode(numbers), id);
        EXPECT_EQ(sqids.decode(id), numbers);
    }
}

TEST(Encoding, MultiInput) {
    sqidscxx::Sqids<> sqids;

    const auto numbers = sqids.numbers({
	    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
	    26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
	    50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
	    74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
	    98, 99
    });

    const auto output = sqids.decode(sqids.encode(numbers));

    EXPECT_EQ(numbers, output);
}

TEST(Encoding, NoNumbers) {
    sqidscxx::Sqids<> sqids;

    EXPECT_EQ(sqids.encode({}), "");
}

TEST(Decoding, EmptyString) {
    sqidscxx::Sqids<> sqids;

    EXPECT_EQ(sqids.decode(""), sqids.numbers({}));
}

TEST(Decoding, DecodingAnIdWithAnInvalidCharacter) {
    sqidscxx::Sqids<> sqids;

    EXPECT_EQ(sqids.decode("*"), sqids.numbers({}));
}

TEST(Decoding, DecodingAnInvalidIdWithARepeatingReservedCharacter) {
    sqidscxx::Sqids<> sqids;

    EXPECT_EQ(sqids.decode("fff"), sqids.numbers({}));
}

TEST(Decoding, EncodeOutOfRangeNumbers) {
    sqidscxx::Sqids<int16_t> sqids;

    ASSERT_THROW(sqids.encode({ sqids.minValue - 1 }), std::runtime_error);
    ASSERT_THROW(sqids.encode({ static_cast<int16_t>(sqids.maxValue + 1) }), std::runtime_error);
}
