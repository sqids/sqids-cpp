#include <gtest/gtest.h>
#include <sqids/sqids.hpp>

TEST(MinLength, Simple) {
    sqidscxx::Sqids<> sqids({ minLength: static_cast<uint8_t>(sqidscxx::SqidsOptions().alphabet.size()) });

    const auto numbers(sqids.numbers({ 1, 2, 3 }));
    const auto id = "86Rf07xd4zBmiJXQG6otHEbew02c3PWsUOLZxADhCpKj7aVFv9I8RquYrNlSTM";

    EXPECT_EQ(sqids.encode(numbers), id);
    EXPECT_EQ(sqids.decode(id), numbers);
}

TEST(MinLength, Incremental) {
    sqidscxx::Sqids<> sqids;

    const auto numbers(sqids.numbers({ 1, 2, 3 }));

    sqidscxx::SqidsOptions defaultOptions;

    const std::vector<std::pair<size_t, std::string>> pairs = {
        { 6, "86Rf07" },
        { 7, "86Rf07x" },
        { 8, "86Rf07xd" },
        { 9, "86Rf07xd4" },
        { 10, "86Rf07xd4z" },
        { 11, "86Rf07xd4zB" },
        { 12, "86Rf07xd4zBm" },
        { 13, "86Rf07xd4zBmi" },
        { defaultOptions.alphabet.size() + 0, 
            "86Rf07xd4zBmiJXQG6otHEbew02c3PWsUOLZxADhCpKj7aVFv9I8RquYrNlSTM" },
        { defaultOptions.alphabet.size() + 1, 
            "86Rf07xd4zBmiJXQG6otHEbew02c3PWsUOLZxADhCpKj7aVFv9I8RquYrNlSTMy" },
        { defaultOptions.alphabet.size() + 2, 
            "86Rf07xd4zBmiJXQG6otHEbew02c3PWsUOLZxADhCpKj7aVFv9I8RquYrNlSTMyf" },
        { defaultOptions.alphabet.size() + 3, 
            "86Rf07xd4zBmiJXQG6otHEbew02c3PWsUOLZxADhCpKj7aVFv9I8RquYrNlSTMyf1" }
    };

    for (auto pair : pairs) {
        const auto minLength = pair.first;
        const auto id = pair.second;

        sqidscxx::Sqids<> sqids({ minLength: static_cast<uint8_t>(minLength) });

        EXPECT_EQ(sqids.encode(numbers), id);
        EXPECT_EQ(sqids.encode(numbers).size(), minLength);
        EXPECT_EQ(sqids.decode(id), numbers);
    }
}

TEST(MinLength, IncrementalNumbers) {
    using IdAndNumbersPairs = std::vector<std::pair<std::string, std::vector<uint64_t>>>;

    sqidscxx::Sqids<> sqids({ minLength: static_cast<uint8_t>(sqidscxx::SqidsOptions().alphabet.size()) });

    const IdAndNumbersPairs pairs = {
        { "SvIzsqYMyQwI3GWgJAe17URxX8V924Co0DaTZLtFjHriEn5bPhcSkfmvOslpBu", { 0, 0 } },
        { "n3qafPOLKdfHpuNw3M61r95svbeJGk7aAEgYn4WlSjXURmF8IDqZBy0CT2VxQc", { 0, 1 } },
        { "tryFJbWcFMiYPg8sASm51uIV93GXTnvRzyfLleh06CpodJD42B7OraKtkQNxUZ", { 0, 2 } },
        { "eg6ql0A3XmvPoCzMlB6DraNGcWSIy5VR8iYup2Qk4tjZFKe1hbwfgHdUTsnLqE", { 0, 3 } },
        { "rSCFlp0rB2inEljaRdxKt7FkIbODSf8wYgTsZM1HL9JzN35cyoqueUvVWCm4hX", { 0, 4 } },
        { "sR8xjC8WQkOwo74PnglH1YFdTI0eaf56RGVSitzbjuZ3shNUXBrqLxEJyAmKv2", { 0, 5 } },
        { "uY2MYFqCLpgx5XQcjdtZK286AwWV7IBGEfuS9yTmbJvkzoUPeYRHr4iDs3naN0", { 0, 6 } },
        { "74dID7X28VLQhBlnGmjZrec5wTA1fqpWtK4YkaoEIM9SRNiC3gUJH0OFvsPDdy", { 0, 7 } },
        { "30WXpesPhgKiEI5RHTY7xbB1GnytJvXOl2p0AcUjdF6waZDo9Qk8VLzMuWrqCS", { 0, 8 } },
        { "moxr3HqLAK0GsTND6jowfZz3SUx7cQ8aC54Pl1RbIvFXmEJuBMYVeW9yrdOtin", { 0, 9 } }
    };

    for (auto pair : pairs) {
        const auto id = pair.first;
        const auto numbers = pair.second;

        EXPECT_EQ(sqids.encode(numbers), id);
        EXPECT_EQ(sqids.decode(id), numbers);
    }
}

TEST(MinLength, MinLengths) {
    using Numbers = std::vector<uint64_t>;

    sqidscxx::Sqids<> sqids;

    std::vector<size_t> minLengths = { 0, 1, 5, 10, sqidscxx::SqidsOptions().alphabet.size() };
    const std::vector<Numbers> numbersList = {
      { 0 },
      { 0, 0, 0, 0, 0 },
      { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
      { 100, 200, 300 },
      { 1'000, 2'000, 3'000 },
      { 1'000'000 },
      { sqids.maxValue } 
    };

    for (auto minLength : minLengths) {
        for (auto numbers : numbersList) {
            sqidscxx::Sqids<> sqids({ minLength: static_cast<uint8_t>(minLength) });

            const auto id = sqids.encode(numbers);
            const auto output = sqids.decode(id);

            EXPECT_EQ(output, numbers);
            EXPECT_GE(id.size(), minLength);
        }
    }
}
