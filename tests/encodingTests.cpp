#include <gtest/gtest.h>
#include <sqids/sqids.hpp>

TEST(Encoding, Simple) {
    sqidscxx::Sqids<> sqids;

    const auto numbers(sqids.numbers({ 1, 2, 3 }));
    const auto id = "86Rf07";

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
        { "bM", { 0 } },
	{ "Uk", { 1 } },
	{ "gb", { 2 } },
	{ "Ef", { 3 } },
	{ "Vq", { 4 } },
	{ "uw", { 5 } },
	{ "OI", { 6 } },
	{ "AX", { 7 } },
	{ "p6", { 8 } },
	{ "nJ", { 9 } }
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
        { "SvIz", { 0, 0 } },
	{ "n3qa", { 0, 1 } },
	{ "tryF", { 0, 2 } },
	{ "eg6q", { 0, 3 } },
	{ "rSCF", { 0, 4 } },
	{ "sR8x", { 0, 5 } },
	{ "uY2M", { 0, 6 } },
	{ "74dI", { 0, 7 } },
	{ "30WX", { 0, 8 } },
	{ "moxr", { 0, 9 } }
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
        { "SvIz", { 0, 0 } },
	{ "nWqP", { 1, 0 } },
	{ "tSyw", { 2, 0 } },
	{ "eX68", { 3, 0 } },
	{ "rxCY", { 4, 0 } },
	{ "sV8a", { 5, 0 } },
	{ "uf2K", { 6, 0 } },
	{ "7Cdk", { 7, 0 } },
	{ "3aWP", { 8, 0 } },
	{ "m2xn", { 9, 0 } }
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

TEST(Decoding, EncodeOutOfRangeNumbers) {
    sqidscxx::Sqids<int16_t> sqids;

    ASSERT_THROW(sqids.encode({ -1 }), std::runtime_error);
    ASSERT_THROW(sqids.encode({ static_cast<int16_t>(sqids.maxValue + 1) }), std::runtime_error);
}
