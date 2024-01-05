// Copyright (c) 2023-present Sqids maintainers.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

///
/// @mainpage
///
/// # sqids
///
/// Sqids (_pronounced "squids"_) is a small library that lets you generate
/// URL-friendly IDs from numbers. It is fast and supports decoding IDs back
/// into numbers for database lookups.
///

///
/// @file   sqids.hpp
/// @link   https://github.com/sqids/sqids-cpp
/// @author Heikki Johannes Hildén
///
#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "blocklist.hpp"

namespace sqidscxx
{
///
/// @struct SqidsOptions
///
/// @brief Options accepted by the `Sqids` constructor.
//
/// All options are optional. Fields that aren't explicitly specified take
/// default values.
///
struct SqidsOptions
{
    ///
    /// The set of characters to use for encoding and decoding IDs.
    ///
    std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    ///
    /// The minimum allowed length of IDs.
    ///
    uint8_t minLength = 0;

    ///
    /// A list of words that must never appear in IDs.
    ///
    std::set<std::string> blocklist = std::set<std::string>(DEFAULT_BLOCKLIST);
};

///
/// @class Sqids
///
/// @brief The main class of this library, repsonsible for encoding and
///        decoding of IDs.
///
/// Typical use:
///
/// @code
/// sqidscxx::Sqids sqids;
/// auto id = sqids.encode({ 1, 2, 3 });
/// // id = "8QRLaD"
/// @endcode
///
/// Or to decode an ID:
///
/// @code
/// sqidscxx::Sqids sqids;
/// auto numbers = sqids.decode("8QRLaD");
/// // numbers = { 1, 2, 3 }
/// @endcode
///
template<typename T = uint64_t>
class Sqids
{
public:
    explicit Sqids(const SqidsOptions& options = {});
    virtual ~Sqids() = default;

    static constexpr std::vector<T> numbers(const std::initializer_list<T>& numbers);
    static constexpr std::vector<T> numbers(std::initializer_list<T>&& numbers);

    bool containsMultibyteCharacters(const std::string& input) const;

    std::string encode(const std::vector<T>& numbers) const;
    std::vector<T> decode(std::string_view id) const;

    static constexpr T maxValue = std::numeric_limits<T>::max();

private:
    struct Encoder
    {
        Encoder(const Sqids<T>* _sqids, const std::vector<T>& _numbers);

        const Sqids<T>* const sqids;
        std::vector<T> numbers;

        std::string run(unsigned int increment = 0);
    };

    std::string lowercaseString(const std::string& input) const;

    void shuffle(std::string& alphabet) const;

    std::string toId(T number, const std::string& alphabet) const;
    T toNumber(const std::string& id, const std::string& alphabet) const;
    bool isBlockedId(const std::string& id) const;

    std::string           _alphabet;
    std::set<std::string> _blocklist;
    uint8_t               _minLength;
};

///
/// Create a `std::vector` of numbers that can be passed to the encode()
/// function. This is a compile-time convenience function that can be used to
/// make client code more readable. For example; the following doesn't work:
///
/// @code
/// auto numbers = { 1, 2, 3 };
/// auto id = sqids.encode(numbers);
/// @endcode
///
/// But this does:
///
/// @code
/// auto numbers = sqids.numbers({ 1, 2, 3 });
/// auto id = sqids.encode(numbers);
/// @endcode
///
/// @param numbers The numbers to encode.
/// @return        A vector of numbers tailored for this sqids instance.
///
template<typename T>
inline constexpr std::vector<T> Sqids<T>::numbers(const std::initializer_list<T>& values)
{
    return std::vector<T>(values);
}

///
/// @overload
///
template<typename T>
inline constexpr std::vector<T> Sqids<T>::numbers(std::initializer_list<T>&& values)
{
    return std::vector<T>(std::move(values));
}

template<typename T>
inline std::string Sqids<T>::lowercaseString(const std::string& input) const
{
    std::string lowercaseString;
    lowercaseString.resize(input.size());
    std::transform(input.cbegin(), input.cend(), lowercaseString.begin(), ::tolower);
    return lowercaseString;
}

///
/// Sqids constructor.
///
/// @param options  Configuration options for this instance. All options are
///                 optional. Fields that aren't explicitly specified take
///                 default values.
///
///
/// @see SqidsOptions
///
template<typename T>
Sqids<T>::Sqids(const SqidsOptions& options)
  : _alphabet(options.alphabet),
    _minLength(options.minLength)
{
    const size_t alphabetSize = options.alphabet.size();

    // Alphabet cannot contain multibyte characters
    if (containsMultibyteCharacters(options.alphabet)) {
        throw std::runtime_error("Alphabet cannot contain multibyte characters.");
    }

    // Check the length of the alphabet
    if (alphabetSize < 3) {
        throw std::runtime_error("Alphabet length must be at least 3.");
    }

    // Check that all the characters in the alphabet are unique
    if (std::set<char>(options.alphabet.cbegin(), options.alphabet.cend()).size() != alphabetSize) {
        throw std::runtime_error("Alphabet must not contain duplicate characters.");
    }

    const std::string lowercaseAlphabet(lowercaseString(options.alphabet));

    // Clean up blocklist
    for (const std::string& word : options.blocklist) {
        // 1. Remove words with less than 3 characters
        if (word.size() < 3) {
            continue;
        }

        const std::string lowercaseWord = lowercaseString(word);

        // 2. Remove words that contain characters not in the alphabet
        if (!std::all_of(lowercaseWord.cbegin(), lowercaseWord.cend(), [lowercaseAlphabet](auto ch) {
            return lowercaseAlphabet.find(ch) != std::string::npos;
        })) {
            continue;
        }

        // 3. Convert words to lowercase
        _blocklist.insert(lowercaseWord);
    }

    shuffle(_alphabet);
}

template<typename T>
inline bool Sqids<T>::containsMultibyteCharacters(const std::string& input) const
{
    for (unsigned char ch : input) {
        if ((ch >> 7) == 1) {
            return true;
        }
    }
    return false;
}

///
/// Encode a sequence of integers into an ID.
///
/// These are the cases where encoding might fail:
///   - One of the numbers passed is smaller than 0 or greater than `maxValue`
///   - An /n/ number of attempts has been made to re-generated the ID, where
///     /n/ is the length of the alphabet + 1
///
/// @throws std::runtime_error When encoding fails
///
/// @param numbers The integers to encode into an ID
/// @return        The generated ID
///
template<typename T>
std::string Sqids<T>::encode(const std::vector<T>& numbers) const
{
    // If no numbers were passed, return an empty string
    if (numbers.empty()) {
        return "";
    }

    // Don't allow out-of-range numbers
    for (auto it = numbers.cbegin(); it != numbers.cend(); ++it) {
        if (*it < 0 || *it > maxValue) {
            std::ostringstream stream;
            stream << "Encoding supports numbers between 0 and " << maxValue;

            throw std::runtime_error(stream.str());
        }
    }

    return Encoder(this, numbers).run();
}

///
/// Decode an ID back into a sequence of integers. This function will return
/// an empty sequence in the following cases:
///   - The ID is empty
///   - An invalid ID is passed
///   - A non-alphabet character is found in the ID
///
/// @param id  The ID to decode
/// @return    The sequence of integers
///
template<typename T>
typename std::vector<T> Sqids<T>::decode(std::string_view id) const
{
    // If an empty string is given, return an empty sequence
    if (id.empty()) {
        return {};
    }

    // If a character is not in the alphabet, return an empty sequence
    for (auto ch : id) {
        if (_alphabet.find(ch) == std::string::npos) {
            return {};
        }
    }

    std::vector<T> numbers;

    // First character is always the `prefix`
    const auto prefix = id[0];

    // `offset` is the semi-random position that was generated during encoding
    const auto offset = _alphabet.find(prefix);

    // Re-arrange alphabet back into it's original form
    std::string alphabet(_alphabet.substr(offset) + _alphabet.substr(0, offset));

    // Reverse alphabet
    std::reverse(alphabet.begin(), alphabet.end());

    // Remove the prefix character from the ID since it is not needed anymore
    std::string slicedId(id.substr(1));

    // Decode
    while (slicedId.size() > 0) {
        const auto separator = alphabet[0];

        std::vector<std::string> chunks;
        size_t start = 0,
               end = 0;

        // We need the first part to the left of the separator to decode the number
        while ((end = slicedId.find(separator, start)) != std::string::npos) {
            chunks.push_back(slicedId.substr(start, end - start));
            start = end + 1;
        }
        chunks.push_back(slicedId.substr(start));

        if (chunks.size() > 0) {
            if (chunks[0].empty()) {
                return numbers;
            }

            // Decode the number without using the `separator` character
            numbers.push_back(toNumber(chunks[0], alphabet.substr(1)));

            // If this ID has multiple numbers, shuffle the alphabet, just as
            // the encoding function does
            if (chunks.size() > 1) {
                shuffle(alphabet);
            }
        }

        // The `id` is now going to be everything to the right of the `separator`
        slicedId.clear();
        chunks.erase(chunks.begin());

        const size_t chunksCount = chunks.size();
        for (unsigned int i = 0; i < chunksCount; i++) {
            slicedId += chunks[i];
            if (i < chunksCount - 1) {
                slicedId += separator;
            }
        }
    }

    return numbers;
}

template<typename T>
void Sqids<T>::shuffle(std::string& alphabet) const
{
    const auto length = alphabet.size();

    // In-place shuffle which always produces the same result, given the same
    // alphabet
    for (unsigned int i = 0, j = length - 1; j > 0; i++, j--) {
        const auto r = (i * j + alphabet[i] + alphabet[j]) % length;
        std::swap(alphabet[i], alphabet[r]);
    }
}

template<typename T>
std::string Sqids<T>::toId(T number, const std::string& alphabet) const
{
    std::string id;
    const size_t alphabetLength = alphabet.size();

    do {
        id.append(1, alphabet[number % alphabetLength]);
        number = number / alphabetLength;
    } while (number > 0);

    std::reverse(id.begin(), id.end());

    return id;
}

template<typename T>
T Sqids<T>::toNumber(const std::string& id, const std::string& alphabet) const
{
    const size_t alphabetSize = alphabet.size();
    T a = 0;

    for (auto ch : id) {
        a = a * alphabetSize + alphabet.find(ch);
    }

    return a;
}

template<typename T>
bool Sqids<T>::isBlockedId(const std::string& id) const
{
    const size_t idLength = id.size();

    auto lowercaseId(lowercaseString(id));

    for (const std::string& word : _blocklist) {
        const size_t wordLength = word.size();
        // No point in checking words that are longer than the ID
        if (wordLength <= idLength) {
            if (idLength <= 3 || wordLength <= 3) {
                // Short words have to match completely; otherwise, too many matches
                if (lowercaseId == word) {
                    return true;
                }
            } else if (std::any_of(word.cbegin(), word.cend(), ::isdigit)) {
                // Words with leet speak replacements are visible mostly at the
                // ends of the ID
                if (lowercaseId.find(word) == 0 || lowercaseId.rfind(word) == idLength - wordLength) {
                    return true;
                }
            } else if (lowercaseId.find(word) != std::string::npos) {
                // Otherwise, check for blocked word anywhere in the string
                return true;
            }
        }
    }

    return false;
}

template<typename T>
Sqids<T>::Encoder::Encoder(const Sqids<T>* _sqids, const std::vector<T>& _numbers)
  : sqids(_sqids),
    numbers(_numbers)
{
}

template<typename T>
std::string Sqids<T>::Encoder::run(unsigned int increment)
{
    const size_t alphabetSize = sqids->_alphabet.size();

    if (increment > alphabetSize) {
        throw std::runtime_error("Reached max attempts to re-generate the ID.");
    }

    // Get a semi-random offset from input numbers
    auto a = numbers.size();

    for (unsigned int i = 0; i < numbers.size(); i++) {
        const T v = numbers[i];
        a += i + sqids->_alphabet[v % alphabetSize];
    }

    const auto offset = (a + increment) % alphabetSize;

    // Re-arrange alphabet so that second-half goes in front of the first-half
    std::string alphabet(sqids->_alphabet.substr(offset) + sqids->_alphabet.substr(0, offset));

    // `prefix` is the first character in the generated ID, used for randomization
    const auto prefix = alphabet[0];

    // Reverse alphabet
    std::reverse(alphabet.begin(), alphabet.end());

    // The final ID will always have the `prefix` character at the beginning
    std::string id = { prefix };

    // Encode the input array
    for (auto it = numbers.cbegin(); it != numbers.cend(); ++it) {

	// The first character of the alphabet is going to be reserved for the `separator`
	const auto alphabetWithoutSeparator = alphabet.substr(1);

	id += sqids->toId(*it, alphabetWithoutSeparator);

        // If not the last number
        if (std::next(it) != numbers.cend()) {
            // `separator` character is used to isolate numbers within the ID
            id.push_back(alphabet[0]);

	    // Shuffle on every iteration
	    sqids->shuffle(alphabet);
        }
    }

    // Handle `minLength` requirement, if the ID is too short
    if (sqids->_minLength > id.size()) {
        // Append a separator
        id.push_back(alphabet[0]);

        // For decoding: two separators next to each other is what tells us the
        // rest are junk characters
        while (sqids->_minLength - id.size() > 0) {
	    sqids->shuffle(alphabet);
            id += alphabet.substr(0, std::min(sqids->_minLength - id.size(), alphabet.size()));
        }
    }

    // if ID has a blocked word anywhere, restart with a +1 increment
    if (sqids->isBlockedId(id)) {
        return run(increment + 1);
    }

    return id;
}

} // namespace sqidscxx
