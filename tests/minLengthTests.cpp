#include <gtest/gtest.h>
#include <sqids/sqids.hpp>

TEST(MinLength, Simple) {
    sqidscxx::Sqids<> sqids({ minLength: sqidscxx::SqidsOptions().alphabet.size() });

    const auto numbers(sqids.numbers({ 1, 2, 3 }));
    const auto id = "75JILToVsGerOADWmHlY38xvbaNZKQ9wdFS0B6kcMEtnRpgizhjU42qT1cd0dL";

    EXPECT_EQ(sqids.encode(numbers), id);
    EXPECT_EQ(sqids.decode(id), numbers);
}

typedef std::vector<std::pair<std::string, std::vector<uint64_t>>> IdAndNumbersPairs;

TEST(MinLength, IncrementalNumbers) {
    sqidscxx::Sqids<> sqids({ minLength: sqidscxx::SqidsOptions().alphabet.size() });

    const IdAndNumbersPairs pairs = {
        { "jf26PLNeO5WbJDUV7FmMtlGXps3CoqkHnZ8cYd19yIiTAQuvKSExzhrRghBlwf", { 0, 0 } },
        { "vQLUq7zWXC6k9cNOtgJ2ZK8rbxuipBFAS10yTdYeRa3ojHwGnmMV4PDhESI2jL", { 0, 1 } },
        { "YhcpVK3COXbifmnZoLuxWgBQwtjsSaDGAdr0ReTHM16yI9vU8JNzlFq5Eu2oPp", { 0, 2 } },
        { "OTkn9daFgDZX6LbmfxI83RSKetJu0APihlsrYoz5pvQw7GyWHEUcN2jBqd4kJ9", { 0, 3 } },
        { "h2cV5eLNYj1x4ToZpfM90UlgHBOKikQFvnW36AC8zrmuJ7XdRytIGPawqYEbBe", { 0, 4 } },
        { "7Mf0HeUNkpsZOTvmcj836P9EWKaACBubInFJtwXR2DSzgYGhQV5i4lLxoT1qdU", { 0, 5 } },
        { "APVSD1ZIY4WGBK75xktMfTev8qsCJw6oyH2j3OnLcXRlhziUmpbuNEar05QCsI", { 0, 6 } },
        { "P0LUhnlT76rsWSofOeyRGQZv1cC5qu3dtaJYNEXwk8Vpx92bKiHIz4MgmiDOF7", { 0, 7 } },
        { "xAhypZMXYIGCL4uW0te6lsFHaPc3SiD1TBgw5O7bvodzjqUn89JQRfk2Nvm4JI", { 0, 8 } },
        { "94dRPIZ6irlXWvTbKywFuAhBoECQOVMjDJp53s2xeqaSzHY8nc17tmkLGwfGNl", { 0, 9 } }
    };

    for (auto pair : pairs) {
        const auto id = pair.first;
        const auto numbers = pair.second;

        EXPECT_EQ(sqids.encode(numbers), id);
        EXPECT_EQ(sqids.decode(id), numbers);
    }
}

TEST(MinLength, MinLengths) {
    sqidscxx::Sqids<> sqids;

    std::vector<size_t> minLengths = { 0, 1, 5, 10, sqidscxx::SqidsOptions().alphabet.size() };
    const std::vector<sqidscxx::Sqids<>::Numbers> numbersList = {
      { sqids.minValue },
      { 0, 0, 0, 0, 0 },
      { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
      { 100, 200, 300 },
      { 1'000, 2'000, 3'000 },
      { 1'000'000 },
      { sqids.maxValue } };

    for (auto minLength : minLengths) {
        for (auto numbers : numbersList) {
            sqidscxx::Sqids<> sqids({ minLength: minLength });

            const auto id = sqids.encode(numbers);
            const auto output = sqids.decode(id);

            EXPECT_EQ(output, numbers);
            EXPECT_GE(id.size(), minLength);
        }
    }
}

TEST(MinLength, OutOfRangeInvalidMinLength) {
    ASSERT_THROW(sqidscxx::Sqids<>({ minLength: sqidscxx::SqidsOptions().alphabet.size() + 1 }), std::runtime_error);
}
