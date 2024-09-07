#pragma once

#include <optional>
#include <string_view>

namespace bencode {
// TODO: Implement int parsing for the bencode fromat
// Bencode encoded integers have the format `i<number>e`. Your function should return the number.
// The function returns a `std::optional<std::string_view>` to indicate success or failure.
// In the case of success, the optional should have the value of the integer number,
// which might be negative.
//
// We assume that the passed string_view contains only one encoding, and the last character
// of a valid encoding is 'e'. Therefore, you do not need to handle cases like 'i4ei42e'.
//
// Example of a valid encoding: "i42e", which should be parsed to the integer 42.
// Example of a valid encoding: "i-42e", which should be parsed to the integer -42.
//
// Edges cases you might need to look out for:
// - String not starting with 'i', or ending with 'e'
// - Handle empty string
// - Handle if a non-digit number is between 'i' and 'e'
consteval std::optional<int> parse_int(std::string_view str) {
    if (str.empty() || str.at(0) != 'i' || str.at(str.size()-1) != 'e') {
        return {};
    }
    str.remove_prefix(1);
    str.remove_suffix(1);
    // checks if the string was "ie"
    if(str.empty()){
        return {};
    }
   

    int isNegative = 1;
    if (str.at(0) == '-') {
        isNegative = -1;
        str.remove_prefix(1);
    }
    int result = 0;
    //i will do a real mumbo jumbo for the next lines
    //since this is a consteval function stoi atoi and other cool shit to convert char to int does not work so I will mess with the ascii values
    // 0 as int equals to 48 9 equals to 57 and other nums are goes in desecding order between 9 and 0 
    // 48 - 48 = 0 , 57 - 48 = 9 so on so forth 
    // take the value gathered by the calculation and do this result = result * 10 + gathered
    // here comes my code
    for (int x : str){
        if (x < 48 || x > 57) {
            return {};
        }
        result = result * 10 + (x -48);
    }

    return result * isNegative;
}

// TODO: Implement byte string parsing for the bencode fromat
// Bencode byte strings have the format <length>:<string>, where `length` is a number.
// The colon ':' separates the length and the actual string.
// The function returns a `std::optional<std::string_view>` to indicate success or failure.
// In the case of success, the optional should have the value of the string.
// For failure, the optional should be empty.
//
// Example of a valid byte string: "5:hello", where the length is 5, and the string is "hello".
//
// You may assume that all characters in the byte string are ASCII characters.
//
// We do not check for all edge cases, important edgecases your implementation should handle:
// - The byte string doesn't start with a number
// - The byte string doesn't have a colon
// - It is fine for the length to be shorter than the string, just return the string according to the length
// - It is NOT valid for the string to be shorter than the specified length
// - The string may contain colons
consteval std::optional<std::string_view> parse_byte_string(std::string_view str) {
    if (str.empty()) {
        return {};
    }

    int colon = str.find(':');
    if (colon == std::string_view::npos) {
        return {};
    }
    int size = 0;
    for (int x : str.substr(0,colon)){
        if (x < 48 || x > 57) {
            return {};
        }
        size = size * 10 + (x -48);
    }
    str.remove_prefix(colon + 1); // exclude the colon :)
    //if the string is smthng like 4 and size is bigger than the str:
    if(str.empty() || size > str.size()){
        return {};
    }
    return str.substr(0, size);
}
} // namespace bencode
