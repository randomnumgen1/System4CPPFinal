#pragma once
/*
    BigInt
    ------
    Arbitrary-sized integer class for C++.

    Version: 0.5.0-dev
    Released on: 05 October 2020 23:15 IST
    Author: Syed Faheel Ahmad (faheel@live.in)
    Project on GitHub: https://github.com/faheel/BigInt
    License: MIT
*/

/*
    ===========================================================================
    BigInt
    ===========================================================================
    Definition for the BigInt class.
*/

#ifndef BIG_INT_HPP
#define BIG_INT_HPP

#include <iostream>
 
class BigInteger {
    std::string value;
    char sign;

public:
    // Constructors:
    BigInteger();
    BigInteger(const BigInteger&);
    BigInteger(const long long&);
    BigInteger(const std::string&);

    // Assignment operators:
    BigInteger& operator=(const BigInteger&);
    BigInteger& operator=(const long long&);
    BigInteger& operator=(const std::string&);

    // Unary arithmetic operators:
    BigInteger operator+() const;   // unary +
    BigInteger operator-() const;   // unary -

    // Binary arithmetic operators:
    BigInteger operator+(const BigInteger&) const;
    BigInteger operator-(const BigInteger&) const;
    BigInteger operator*(const BigInteger&) const;
    BigInteger operator/(const BigInteger&) const;
    BigInteger operator%(const BigInteger&) const;
    BigInteger operator+(const long long&) const;
    BigInteger operator-(const long long&) const;
    BigInteger operator*(const long long&) const;
    BigInteger operator/(const long long&) const;
    BigInteger operator%(const long long&) const;
    BigInteger operator+(const std::string&) const;
    BigInteger operator-(const std::string&) const;
    BigInteger operator*(const std::string&) const;
    BigInteger operator/(const std::string&) const;
    BigInteger operator%(const std::string&) const;

    // Arithmetic-assignment operators:
    BigInteger& operator+=(const BigInteger&);
    BigInteger& operator-=(const BigInteger&);
    BigInteger& operator*=(const BigInteger&);
    BigInteger& operator/=(const BigInteger&);
    BigInteger& operator%=(const BigInteger&);
    BigInteger& operator+=(const long long&);
    BigInteger& operator-=(const long long&);
    BigInteger& operator*=(const long long&);
    BigInteger& operator/=(const long long&);
    BigInteger& operator%=(const long long&);
    BigInteger& operator+=(const std::string&);
    BigInteger& operator-=(const std::string&);
    BigInteger& operator*=(const std::string&);
    BigInteger& operator/=(const std::string&);
    BigInteger& operator%=(const std::string&);

    // Increment and decrement operators:
    BigInteger& operator++();       // pre-increment
    BigInteger& operator--();       // pre-decrement
    BigInteger operator++(int);     // post-increment
    BigInteger operator--(int);     // post-decrement

    // Relational operators:
    bool operator<(const BigInteger&) const;
    bool operator>(const BigInteger&) const;
    bool operator<=(const BigInteger&) const;
    bool operator>=(const BigInteger&) const;
    bool operator==(const BigInteger&) const;
    bool operator!=(const BigInteger&) const;
    bool operator<(const long long&) const;
    bool operator>(const long long&) const;
    bool operator<=(const long long&) const;
    bool operator>=(const long long&) const;
    bool operator==(const long long&) const;
    bool operator!=(const long long&) const;
    bool operator<(const std::string&) const;
    bool operator>(const std::string&) const;
    bool operator<=(const std::string&) const;
    bool operator>=(const std::string&) const;
    bool operator==(const std::string&) const;
    bool operator!=(const std::string&) const;

    // I/O stream operators:
    friend std::istream& operator>>(std::istream&, BigInteger&);
    friend std::ostream& operator<<(std::ostream&, const BigInteger&);

    // Conversion functions:
    std::string to_string() const;
    int to_int() const;
    long to_long() const;
    long long to_long_long() const;
    std::string  to_hex_string() const;
    // Random number generating functions:
    friend BigInteger big_random(size_t);
    static BigInteger modInverse(BigInteger, BigInteger);

};

#endif  // BIG_INT_HPP


/*
    ===========================================================================
    Utility functions
    ===========================================================================
*/

#ifndef BIG_INT_UTILITY_FUNCTIONS_HPP
#define BIG_INT_UTILITY_FUNCTIONS_HPP

#include <tuple>


/*
    is_valid_number
    ---------------
    Checks whether the given string is a valid integer.
*/

bool is_valid_number(const std::string& num) {
    for (char digit : num)
        if (digit < '0' or digit > '9')
            return false;

    return true;
}


/*
    strip_leading_zeroes
    --------------------
    Strip the leading zeroes from a number represented as a string.
*/

void strip_leading_zeroes(std::string& num) {
    size_t i;
    for (i = 0; i < num.size(); i++)
        if (num[i] != '0')
            break;

    if (i == num.size())
        num = "0";
    else
        num = num.substr(i);
}


/*
    add_leading_zeroes
    ------------------
    Adds a given number of leading zeroes to a string-represented integer `num`.
*/

void add_leading_zeroes(std::string& num, size_t num_zeroes) {
    num = std::string(num_zeroes, '0') + num;
}


/*
    add_trailing_zeroes
    -------------------
    Adds a given number of trailing zeroes to a string-represented integer `num`.
*/

void add_trailing_zeroes(std::string& num, size_t num_zeroes) {
    num += std::string(num_zeroes, '0');
}


/*
    get_larger_and_smaller
    ----------------------
    Identifies the given string-represented integers as `larger` and `smaller`,
    padding the smaller number with leading zeroes to make it equal in length to
    the larger number.
*/

std::tuple<std::string, std::string> get_larger_and_smaller(const std::string& num1,
                                                            const std::string& num2) {
    std::string larger, smaller;
    if (num1.size() > num2.size() or
        (num1.size() == num2.size() and num1 > num2)) {
        larger = num1;
        smaller = num2;
    }
    else {
        larger = num2;
        smaller = num1;
    }

    // pad the smaller number with zeroes
    add_leading_zeroes(smaller, larger.size() - smaller.size());

    return std::make_tuple(larger, smaller);
}


/*
    is_power_of_10
    ----------------------
    Checks whether a string-represented integer is a power of 10.
*/

bool is_power_of_10(const std::string& num) {
    if (num[0] != '1')
        return false;
    for (size_t i = 1; i < num.size(); i++)
        if (num[i] != '0')
            return false;

    return true;    // first digit is 1 and the following digits are all 0
}

#endif  // BIG_INT_UTILITY_FUNCTIONS_HPP


/*
    ===========================================================================
    Random number generating functions for BigInt
    ===========================================================================
*/

#ifndef BIG_INT_RANDOM_FUNCTIONS_HPP
#define BIG_INT_RANDOM_FUNCTIONS_HPP

#include <random>
#include <climits>


// when the number of digits are not specified, a random value is used for it
// which is kept below the following:
const size_t MAX_RANDOM_LENGTH = 1000;


/*
    big_random (num_digits)
    -----------------------
    Returns a random BigInt with a specific number of digits.
*/

BigInteger big_random(size_t num_digits = 0) {
    std::random_device rand_generator;      // true random number generator

    if (num_digits == 0)    // the number of digits were not specified
        // use a random number for it:
        num_digits = 1 + rand_generator() % MAX_RANDOM_LENGTH;

    BigInteger big_rand;
    big_rand.value = "";    // clear value to append digits

    // ensure that the first digit is non-zero
    big_rand.value += std::to_string(1 + rand_generator() % 9);

    while (big_rand.value.size() < num_digits)
        big_rand.value += std::to_string(rand_generator());
    if (big_rand.value.size() != num_digits)
        big_rand.value.erase(num_digits);   // erase extra digits

    return big_rand;
}


#endif  // BIG_INT_RANDOM_FUNCTIONS_HPP


/*
    ===========================================================================
    Constructors
    ===========================================================================
*/

#ifndef BIG_INT_CONSTRUCTORS_HPP
#define BIG_INT_CONSTRUCTORS_HPP



/*
    Default constructor
    -------------------
*/

BigInteger::BigInteger() {
    value = "0";
    sign = '+';
}


/*
    Copy constructor
    ----------------
*/

BigInteger::BigInteger(const BigInteger& num) {
    value = num.value;
    sign = num.sign;
}


/*
    Integer to BigInt
    -----------------
*/

BigInteger::BigInteger(const long long& num) {
    value = std::to_string(std::abs(num));
    if (num < 0)
        sign = '-';
    else
        sign = '+';
}


/*
    String to BigInt
    ----------------
*/
void parse_hex_string(std::string&& str, std::string& magnitude) {
    BigInteger res;
    for (size_t idx = 2; idx < str.size(); idx++) {
        res *= 16;
        char c = str[idx];
        // digit
        if (c >= '0' && c <= '9')
            res += c - '0';
        // lowercase hex
        else if (c >= 'a' && c <= 'f')
            res += c - 'a' + 10;
        // uppercase hex
        else if (c >= 'A' && c <= 'F')
            res += c - 'A' + 10;
        else
            throw std::invalid_argument("Expected a hex integer, got \'" + str + "\'");
    }
    magnitude = res.to_string();
}
void parse_string(std::string&& str, std::string& magnitude) {
    if (str.size() > 1 && str[0] == '0' && str[1] == 'x') {
        parse_hex_string(std::move(str), magnitude);
        return;
    }

    // check if every char is decimal
    for (auto c : str)
        if (c < '0' || c > '9')
            throw std::invalid_argument("Expected a decimal integer, got \'" + str + "\'");

    magnitude = std::move(str);
}
BigInteger::BigInteger(const std::string& num) {
    std::string magnitude = num;
    sign = '+';
    if (magnitude[0] == '+' or magnitude[0] == '-') {
        sign = num[0];
        // delete first char
        magnitude.erase(0, 1);
    }

    parse_string(std::move(magnitude), value);

    strip_leading_zeroes(value);
}
/*
BigInt::BigInt(const std::string& num) {
    if (num[0] == '+' or num[0] == '-') {     // check for sign
        std::string magnitude = num.substr(1);
        if (is_valid_number(magnitude)) {
            value = magnitude;
            sign = num[0];
        }
        else {
            throw std::invalid_argument("Expected an integer, got \'" + num + "\'");
        }
    }
    else {      // if no sign is specified
        if (is_valid_number(num)) {
            value = num;
            sign = '+';    // positive by default
        }
        else {
            throw std::invalid_argument("Expected an integer, got \'" + num + "\'");
        }
    }
    strip_leading_zeroes(value);
}
*/
#endif  // BIG_INT_CONSTRUCTORS_HPP


/*
    ===========================================================================
    Conversion functions for BigInt
    ===========================================================================
*/

#ifndef BIG_INT_CONVERSION_FUNCTIONS_HPP
#define BIG_INT_CONVERSION_FUNCTIONS_HPP


/*
    to_string
    ---------
    Converts a BigInt to a string.
*/

std::string BigInteger::to_string() const {
    // prefix with sign if negative
    return this->sign == '-' ? "-" + this->value : this->value;
}


/*
    to_int
    ------
    Converts a BigInt to an int.
    NOTE: If the BigInt is out of range of an int, stoi() will throw an
    out_of_range exception.
*/

int BigInteger::to_int() const {
    return std::stoi(this->to_string());
}


/*
    to_long
    -------
    Converts a BigInt to a long int.
    NOTE: If the BigInt is out of range of a long int, stol() will throw an
    out_of_range exception.
*/

long BigInteger::to_long() const {
    return std::stol(this->to_string());
}
/*
std::string BigInt::to_hex_string() const {
    std::string dfdsf;
    for (int i = 0; i < this->value.length();i++) {
        std::cout << this->value[i] << std::endl;
    }
    return dfdsf;
}*/
 
/*
std::string BigInt::to_hex_string() const {
    std::cout << sign << ")" << std::endl;
    // Convert the numeric string to a BigInt or integer value
    BigInt num(this->value); // Assuming BigInt can parse the string into a numerical value

    // Use a string to store the hexadecimal representation
    std::string hex_string;
    BigInt zero(0); // Represents 0

    // Loop to convert the number to hexadecimal
    while (num > zero) {
        BigInt remainder = num % 16; // Get the remainder when dividing by 16
        char hex_digit;

        // Map the remainder to a hexadecimal digit
        if (remainder < 10) {
            hex_digit = '0' + remainder.to_int(); // Convert to '0'-'9'
        }
        else {
            hex_digit = 'A' + static_cast<char>(remainder.to_int() - 10); // Convert to 'A'-'F'
        }
       
        hex_string = hex_digit + hex_string; // Add the hex digit to the result
        num = num / 16; // Divide the number by 16
    }

    return hex_string.empty() ? "0" : hex_string; // Handle case where value is 0
}*/
std::string BigInteger::to_hex_string() const {
    // Check if the number is zero
    BigInteger num(this->value); // Parse the value into a BigInt
    if (num == 0) {
        return "0";
    }

    std::string hex_string; // Store the resulting hex string
    BigInteger zero(0);         // Define zero for comparison

    while (num > zero) {
        BigInteger remainder = num % 16; // Get the remainder when dividing by 16
        char hex_digit = (remainder.to_int() < 10)
            ? ('0' + remainder.to_int())
            : ('A' + (remainder.to_int() - 10)); // Map remainder to hex
        hex_string += hex_digit; // Append to hex string
        num = num / 16;          // Divide the number by 16
    }

    // Reverse the string for correct order
    std::reverse(hex_string.begin(), hex_string.end());

    return hex_string;
}
/*
    to_long_long
    ------------
    Converts a BigInt to a long long int.
    NOTE: If the BigInt is out of range of a long long int, stoll() will throw
    an out_of_range exception.
*/

long long BigInteger::to_long_long() const {
    return std::stoll(this->to_string());
}


 BigInteger BigInteger::modInverse(BigInteger a, BigInteger m) {
    BigInteger m0 = m, x0 = 0, x1 = 1;

    // Ensure modular inverse exists
    if (m == 1)
        throw std::invalid_argument("Modulo is 1, modular inverse does not exist.");

    while (a > 1) {
        // q is quotient
        BigInteger q = a / m;
        BigInteger t = m;

        // Update m and a (remainder and dividend respectively)
        m = a % m;
        a = t;
        t = x0;

        // Update x0 and x1
        x0 = x1 - q * x0;
        x1 = t;
    }

    // Ensure x1 is positive
    if (x1 < 0)
        x1 += m0;

    return x1;
}













#endif  // BIG_INT_CONVERSION_FUNCTIONS_HPP


/*
    ===========================================================================
    Assignment operators
    ===========================================================================
*/

#ifndef BIG_INT_ASSIGNMENT_OPERATORS_HPP
#define BIG_INT_ASSIGNMENT_OPERATORS_HPP



/*
    BigInt = BigInt
    ---------------
*/

BigInteger& BigInteger::operator=(const BigInteger& num) {
    value = num.value;
    sign = num.sign;

    return *this;
}


/*
    BigInt = Integer
    ----------------
*/

BigInteger& BigInteger::operator=(const long long& num) {
    BigInteger temp(num);
    value = temp.value;
    sign = temp.sign;

    return *this;
}


/*
    BigInt = String
    ---------------
*/

BigInteger& BigInteger::operator=(const  std::string& num) {
    BigInteger temp(num);
    value = temp.value;
    sign = temp.sign;

    return *this;
}

#endif  // BIG_INT_ASSIGNMENT_OPERATORS_HPP


/*
    ===========================================================================
    Unary arithmetic operators
    ===========================================================================
*/

#ifndef BIG_INT_UNARY_ARITHMETIC_OPERATORS_HPP
#define BIG_INT_UNARY_ARITHMETIC_OPERATORS_HPP



/*
    +BigInt
    -------
    Returns the value of a BigInt.
    NOTE: This function does not return the absolute value. To get the absolute
    value of a BigInt, use the `abs` function.
*/

BigInteger BigInteger::operator+() const {
    return *this;
}


/*
    -BigInt
    -------
    Returns the negative of a BigInt.
*/

BigInteger BigInteger::operator-() const {
    BigInteger temp;

    temp.value = value;
    if (value != "0") {
        if (sign == '+')
            temp.sign = '-';
        else
            temp.sign = '+';
    }

    return temp;
}

#endif  // BIG_INT_UNARY_ARITHMETIC_OPERATORS_HPP


/*
    ===========================================================================
    Relational operators
    ===========================================================================
    All operators depend on the '<' and/or '==' operator(s).
*/

#ifndef BIG_INT_RELATIONAL_OPERATORS_HPP
#define BIG_INT_RELATIONAL_OPERATORS_HPP



/*
    BigInt == BigInt
    ----------------
*/

bool BigInteger::operator==(const BigInteger& num) const {
    return (sign == num.sign) and (value == num.value);
}


/*
    BigInt != BigInt
    ----------------
*/

bool BigInteger::operator!=(const BigInteger& num) const {
    return !(*this == num);
}


/*
    BigInt < BigInt
    ---------------
*/

bool BigInteger::operator<(const BigInteger& num) const {
    if (sign == num.sign) {
        if (sign == '+') {
            if (value.length() == num.value.length())
                return value < num.value;
            else
                return value.length() < num.value.length();
        }
        else
            return -(*this) > -num;
    }
    else
        return sign == '-';
}


/*
    BigInt > BigInt
    ---------------
*/

bool BigInteger::operator>(const BigInteger& num) const {
    return !((*this < num) or (*this == num));
}


/*
    BigInt <= BigInt
    ----------------
*/

bool BigInteger::operator<=(const BigInteger& num) const {
    return (*this < num) or (*this == num);
}


/*
    BigInt >= BigInt
    ----------------
*/

bool BigInteger::operator>=(const BigInteger& num) const {
    return !(*this < num);
}


/*
    BigInt == Integer
    -----------------
*/

bool BigInteger::operator==(const long long& num) const {
    return *this == BigInteger(num);
}


/*
    Integer == BigInt
    -----------------
*/

bool operator==(const long long& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) == rhs;
}


/*
    BigInt != Integer
    -----------------
*/

bool BigInteger::operator!=(const long long& num) const {
    return !(*this == BigInteger(num));
}


/*
    Integer != BigInt
    -----------------
*/

bool operator!=(const long long& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) != rhs;
}


/*
    BigInt < Integer
    ----------------
*/

bool BigInteger::operator<(const long long& num) const {
    return *this < BigInteger(num);
}


/*
    Integer < BigInt
    ----------------
*/

bool operator<(const long long& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) < rhs;
}


/*
    BigInt > Integer
    ----------------
*/

bool BigInteger::operator>(const long long& num) const {
    return *this > BigInteger(num);
}


/*
    Integer > BigInt
    ----------------
*/

bool operator>(const long long& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) > rhs;
}


/*
    BigInt <= Integer
    -----------------
*/

bool BigInteger::operator<=(const long long& num) const {
    return !(*this > BigInteger(num));
}


/*
    Integer <= BigInt
    -----------------
*/

bool operator<=(const long long& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) <= rhs;
}


/*
    BigInt >= Integer
    -----------------
*/

bool BigInteger::operator>=(const long long& num) const {
    return !(*this < BigInteger(num));
}


/*
    Integer >= BigInt
    -----------------
*/

bool operator>=(const long long& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) >= rhs;
}


/*
    BigInt == String
    ----------------
*/

bool BigInteger::operator==(const std::string& num) const {
    return *this == BigInteger(num);
}


/*
    String == BigInt
    ----------------
*/

bool operator==(const std::string& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) == rhs;
}


/*
    BigInt != String
    ----------------
*/

bool BigInteger::operator!=(const std::string& num) const {
    return !(*this == BigInteger(num));
}


/*
    String != BigInt
    ----------------
*/

bool operator!=(const std::string& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) != rhs;
}


/*
    BigInt < String
    ---------------
*/

bool BigInteger::operator<(const std::string& num) const {
    return *this < BigInteger(num);
}


/*
    String < BigInt
    ---------------
*/

bool operator<(const std::string& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) < rhs;
}


/*
    BigInt > String
    ---------------
*/

bool BigInteger::operator>(const std::string& num) const {
    return *this > BigInteger(num);
}


/*
    String > BigInt
    ---------------
*/

bool operator>(const std::string& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) > rhs;
}


/*
    BigInt <= String
    ----------------
*/

bool BigInteger::operator<=(const std::string& num) const {
    return !(*this > BigInteger(num));
}


/*
    String <= BigInt
    ----------------
*/

bool operator<=(const std::string& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) <= rhs;
}


/*
    BigInt >= String
    ----------------
*/

bool BigInteger::operator>=(const std::string& num) const {
    return !(*this < BigInteger(num));
}


/*
    String >= BigInt
    ----------------
*/

bool operator>=(const std::string& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) >= rhs;
}

#endif  // BIG_INT_RELATIONAL_OPERATORS_HPP


/*
    ===========================================================================
    Math functions for BigInt
    ===========================================================================
*/

#ifndef BIG_INT_MATH_FUNCTIONS_HPP
#define BIG_INT_MATH_FUNCTIONS_HPP

#include <string>



/*
    abs
    ---
    Returns the absolute value of a BigInt.
*/

BigInteger abs(const BigInteger& num) {
    return num < 0 ? -num : num;
}


/*
    big_pow10
    ---------
    Returns a BigInt equal to 10^exp.
    NOTE: exponent should be a non-negative integer.
*/

BigInteger big_pow10(size_t exp) {
    return BigInteger("1" + std::string(exp, '0'));
}


/*
    pow (BigInt)
    ------------
    Returns a BigInt equal to base^exp.
*/

BigInteger pow(const BigInteger& base, int exp) {
    if (exp < 0) {
        if (base == 0)
            throw std::logic_error("Cannot divide by zero");
        return abs(base) == 1 ? base : 0;
    }
    if (exp == 0) {
        if (base == 0)
            throw std::logic_error("Zero cannot be raised to zero");
        return 1;
    }

    BigInteger result = base, result_odd = 1;
    while (exp > 1) {
        if (exp % 2)
            result_odd *= result;
        result *= result;
        exp /= 2;
    }

    return result * result_odd;
}


/*
    pow (Integer)
    -------------
    Returns a BigInt equal to base^exp.
*/

BigInteger pow(const long long& base, int exp) {
    return pow(BigInteger(base), exp);
}


/*
    pow (String)
    ------------
    Returns a BigInt equal to base^exp.
*/

BigInteger pow(const std::string& base, int exp) {
    return pow(BigInteger(base), exp);

}


/*
    sqrt
    ----
    Returns the positive integer square root of a BigInt using Newton's method.
    NOTE: the input must be non-negative.
*/

BigInteger sqrt(const BigInteger& num) {
    if (num < 0)
        throw std::invalid_argument("Cannot compute square root of a negative integer");

    // Optimisations for small inputs:
    if (num == 0)
        return 0;
    else if (num < 4)
        return 1;
    else if (num < 9)
        return 2;
    else if (num < 16)
        return 3;

    BigInteger sqrt_prev = -1;
    // The value for `sqrt_current` is chosen close to that of the actual
    // square root.
    // Since a number's square root has at least one less than half as many
    // digits as the number,
    //     sqrt_current = 10^(half_the_digits_in_num - 1)
    BigInteger sqrt_current = big_pow10(num.to_string().size() / 2 - 1);

    while (abs(sqrt_current - sqrt_prev) > 1) {
        sqrt_prev = sqrt_current;
        sqrt_current = (num / sqrt_prev + sqrt_prev) / 2;
    }

    return sqrt_current;
}


/*
    gcd(BigInt, BigInt)
    -------------------
    Returns the greatest common divisor (GCD, a.k.a. HCF) of two BigInts using
    Euclid's algorithm.
*/

BigInteger gcd(const BigInteger& num1, const BigInteger& num2) {
    BigInteger abs_num1 = abs(num1);
    BigInteger abs_num2 = abs(num2);

    // base cases:
    if (abs_num2 == 0)
        return abs_num1;    // gcd(a, 0) = |a|
    if (abs_num1 == 0)
        return abs_num2;    // gcd(0, a) = |a|

    BigInteger remainder = abs_num2;
    while (remainder != 0) {
        remainder = abs_num1 % abs_num2;
        abs_num1 = abs_num2;    // previous remainder
        abs_num2 = remainder;   // current remainder
    }

    return abs_num1;
}


/*
    gcd(BigInt, Integer)
    --------------------
*/

BigInteger gcd(const BigInteger& num1, const long long& num2) {
    return gcd(num1, BigInteger(num2));
}


/*
    gcd(BigInt, String)
    -------------------
*/

BigInteger gcd(const BigInteger& num1, const std::string& num2) {
    return gcd(num1, BigInteger(num2));
}


/*
    gcd(Integer, BigInt)
    --------------------
*/

BigInteger gcd(const long long& num1, const BigInteger& num2) {
    return gcd(BigInteger(num1), num2);
}


/*
    gcd(String, BigInt)
    -------------------
*/

BigInteger gcd(const std::string& num1, const BigInteger& num2) {
    return gcd(BigInteger(num1), num2);
}


/*
    lcm(BigInt, BigInt)
    -------------------
    Returns the least common multiple (LCM) of two BigInts.
*/

BigInteger lcm(const BigInteger& num1, const BigInteger& num2) {
    if (num1 == 0 or num2 == 0)
        return 0;

    return abs(num1 * num2) / gcd(num1, num2);
}


/*
    lcm(BigInt, Integer)
    --------------------
*/

BigInteger lcm(const BigInteger& num1, const long long& num2) {
    return lcm(num1, BigInteger(num2));
}


/*
    lcm(BigInt, String)
    -------------------
*/

BigInteger lcm(const BigInteger& num1, const std::string& num2) {
    return lcm(num1, BigInteger(num2));
}


/*
    lcm(Integer, BigInt)
    --------------------
*/

BigInteger lcm(const long long& num1, const BigInteger& num2) {
    return lcm(BigInteger(num1), num2);
}


/*
    lcm(String, BigInt)
    -------------------
*/

BigInteger lcm(const std::string& num1, const BigInteger& num2) {
    return lcm(BigInteger(num1), num2);
}


#endif  // BIG_INT_MATH_FUNCTIONS_HPP
 

/*
    ===========================================================================
    Binary arithmetic operators
    ===========================================================================
*/

#ifndef BIG_INT_BINARY_ARITHMETIC_OPERATORS_HPP
#define BIG_INT_BINARY_ARITHMETIC_OPERATORS_HPP

#include <climits>
#include <cmath>
#include <string>


const long long FLOOR_SQRT_LLONG_MAX = 3037000499;


/*
    BigInt + BigInt
    ---------------
    The operand on the RHS of the addition is `num`.
*/

BigInteger BigInteger::operator+(const BigInteger& num) const {
    // if the operands are of opposite signs, perform subtraction
    if (this->sign == '+' and num.sign == '-') {
        BigInteger rhs = num;
        rhs.sign = '+';
        return *this - rhs;
    }
    else if (this->sign == '-' and num.sign == '+') {
        BigInteger lhs = *this;
        lhs.sign = '+';
        return -(lhs - num);
    }

    // identify the numbers as `larger` and `smaller`
    std::string larger, smaller;
    std::tie(larger, smaller) = get_larger_and_smaller(this->value, num.value);

    BigInteger result;      // the resultant sum
    result.value = "";  // the value is cleared as the digits will be appended
    short carry = 0, sum;
    // add the two values
    for (long i = larger.size() - 1; i >= 0; i--) {
        sum = larger[i] - '0' + smaller[i] - '0' + carry;
        result.value = std::to_string(sum % 10) + result.value;
        carry = sum / (short)10;
    }
    if (carry)
        result.value = std::to_string(carry) + result.value;

    // if the operands are negative, the result is negative
    if (this->sign == '-' and result.value != "0")
        result.sign = '-';

    return result;
}


/*
    BigInt - BigInt
    ---------------
    The operand on the RHS of the subtraction is `num`.
*/

BigInteger BigInteger::operator-(const BigInteger& num) const {
    // if the operands are of opposite signs, perform addition
    if (this->sign == '+' and num.sign == '-') {
        BigInteger rhs = num;
        rhs.sign = '+';
        return *this + rhs;
    }
    else if (this->sign == '-' and num.sign == '+') {
        BigInteger lhs = *this;
        lhs.sign = '+';
        return -(lhs + num);
    }

    BigInteger result;      // the resultant difference
    // identify the numbers as `larger` and `smaller`
    std::string larger, smaller;
    if (abs(*this) > abs(num)) {
        larger = this->value;
        smaller = num.value;

        if (this->sign == '-')      // -larger - -smaller = -result
            result.sign = '-';
    }
    else {
        larger = num.value;
        smaller = this->value;

        if (num.sign == '+')        // smaller - larger = -result
            result.sign = '-';
    }
    // pad the smaller number with zeroes
    add_leading_zeroes(smaller, larger.size() - smaller.size());

    result.value = "";  // the value is cleared as the digits will be appended
    short difference;
    long i, j;
    // subtract the two values
    for (i = larger.size() - 1; i >= 0; i--) {
        difference = larger[i] - smaller[i];
        if (difference < 0) {
            for (j = i - 1; j >= 0; j--) {
                if (larger[j] != '0') {
                    larger[j]--;    // borrow from the j-th digit
                    break;
                }
            }
            j++;
            while (j != i) {
                larger[j] = '9';    // add the borrow and take away 1
                j++;
            }
            difference += 10;   // add the borrow
        }
        result.value = std::to_string(difference) + result.value;
    }
    strip_leading_zeroes(result.value);

    // if the result is 0, set its sign as +
    if (result.value == "0")
        result.sign = '+';

    return result;
}


/*
    BigInt * BigInt
    ---------------
    Computes the product of two BigInts using Karatsuba's algorithm.
    The operand on the RHS of the product is `num`.
*/

BigInteger BigInteger::operator*(const BigInteger& num) const {
    if (*this == 0 or num == 0)
        return BigInteger(0);
    if (*this == 1)
        return num;
    if (num == 1)
        return *this;

    BigInteger product;
    if (abs(*this) <= FLOOR_SQRT_LLONG_MAX and abs(num) <= FLOOR_SQRT_LLONG_MAX)
        product = std::stoll(this->value) * std::stoll(num.value);
    else if (is_power_of_10(this->value)) { // if LHS is a power of 10 do optimised operation 
        product.value = num.value;
        product.value.append(this->value.begin() + 1, this->value.end());
    }
    else if (is_power_of_10(num.value)) { // if RHS is a power of 10 do optimised operation 
        product.value = this->value;
        product.value.append(num.value.begin() + 1, num.value.end());
    }
    else {
        // identify the numbers as `larger` and `smaller`
        std::string larger, smaller;
        std::tie(larger, smaller) = get_larger_and_smaller(this->value, num.value);

        size_t half_length = larger.size() / 2;
        auto half_length_ceil = (size_t)ceil(larger.size() / 2.0);

        BigInteger num1_high, num1_low;
        num1_high = larger.substr(0, half_length);
        num1_low = larger.substr(half_length);

        BigInteger num2_high, num2_low;
        num2_high = smaller.substr(0, half_length);
        num2_low = smaller.substr(half_length);

        strip_leading_zeroes(num1_high.value);
        strip_leading_zeroes(num1_low.value);
        strip_leading_zeroes(num2_high.value);
        strip_leading_zeroes(num2_low.value);

        BigInteger prod_high, prod_mid, prod_low;
        prod_high = num1_high * num2_high;
        prod_low = num1_low * num2_low;
        prod_mid = (num1_high + num1_low) * (num2_high + num2_low)
            - prod_high - prod_low;

        add_trailing_zeroes(prod_high.value, 2 * half_length_ceil);
        add_trailing_zeroes(prod_mid.value, half_length_ceil);

        strip_leading_zeroes(prod_high.value);
        strip_leading_zeroes(prod_mid.value);
        strip_leading_zeroes(prod_low.value);

        product = prod_high + prod_mid + prod_low;
    }
    strip_leading_zeroes(product.value);

    if (this->sign == num.sign)
        product.sign = '+';
    else
        product.sign = '-';

    return product;
}


/*
    divide
    ------
    Helper function that returns the quotient and remainder on dividing the
    dividend by the divisor, when the divisor is 1 to 10 times the dividend.
*/

std::tuple<BigInteger, BigInteger> divide(const BigInteger& dividend, const BigInteger& divisor) {
    BigInteger quotient, remainder, temp;

    temp = divisor;
    quotient = 1;
    while (temp < dividend) {
        quotient++;
        temp += divisor;
    }
    if (temp > dividend) {
        quotient--;
        remainder = dividend - (temp - divisor);
    }

    return std::make_tuple(quotient, remainder);
}


/*
    BigInt / BigInt
    ---------------
    Computes the quotient of two BigInts using the long-division method.
    The operand on the RHS of the division (the divisor) is `num`.
*/

BigInteger BigInteger::operator/(const BigInteger& num) const {
    BigInteger abs_dividend = abs(*this);
    BigInteger abs_divisor = abs(num);

    if (num == 0)
        throw std::logic_error("Attempted division by zero");
    if (abs_dividend < abs_divisor)
        return BigInteger(0);
    if (num == 1)
        return *this;
    if (num == -1)
        return -(*this);

    BigInteger quotient;
    if (abs_dividend <= LLONG_MAX and abs_divisor <= LLONG_MAX)
        quotient = std::stoll(abs_dividend.value) / std::stoll(abs_divisor.value);
    else if (abs_dividend == abs_divisor)
        quotient = 1;
    else if (is_power_of_10(abs_divisor.value)) { // if divisor is a power of 10 do optimised calculation
        size_t digits_in_quotient = abs_dividend.value.size() - abs_divisor.value.size() + 1;
        quotient.value = abs_dividend.value.substr(0, digits_in_quotient);
    }
    else {
        quotient.value = "";    // the value is cleared as digits will be appended
        BigInteger chunk, chunk_quotient, chunk_remainder;
        size_t chunk_index = 0;
        chunk_remainder.value = abs_dividend.value.substr(chunk_index, abs_divisor.value.size() - 1);
        chunk_index = abs_divisor.value.size() - 1;
        while (chunk_index < abs_dividend.value.size()) {
            chunk.value = chunk_remainder.value.append(1, abs_dividend.value[chunk_index]);
            chunk_index++;
            while (chunk < abs_divisor) {
                quotient.value += "0";
                if (chunk_index < abs_dividend.value.size()) {
                    chunk.value.append(1, abs_dividend.value[chunk_index]);
                    chunk_index++;
                }
                else
                    break;
            }
            if (chunk == abs_divisor) {
                quotient.value += "1";
                chunk_remainder = 0;
            }
            else if (chunk > abs_divisor) {
                strip_leading_zeroes(chunk.value);
                std::tie(chunk_quotient, chunk_remainder) = divide(chunk, abs_divisor);
                quotient.value += chunk_quotient.value;
            }
        }
    }
    strip_leading_zeroes(quotient.value);

    if (this->sign == num.sign)
        quotient.sign = '+';
    else
        quotient.sign = '-';

    return quotient;
}


/*
    BigInt % BigInt
    ---------------
    Computes the modulo (remainder on division) of two BigInts.
    The operand on the RHS of the modulo (the divisor) is `num`.
*/

BigInteger BigInteger::operator%(const BigInteger& num) const {
    BigInteger abs_dividend = abs(*this);
    BigInteger abs_divisor = abs(num);

    if (abs_divisor == 0)
        throw std::logic_error("Attempted division by zero");
    if (abs_divisor == 1 or abs_divisor == abs_dividend)
        return BigInteger(0);

    BigInteger remainder;
    if (abs_dividend <= LLONG_MAX and abs_divisor <= LLONG_MAX)
        remainder = std::stoll(abs_dividend.value) % std::stoll(abs_divisor.value);
    else if (abs_dividend < abs_divisor)
        remainder = abs_dividend;
    else if (is_power_of_10(num.value)) { // if num is a power of 10 use optimised calculation
        size_t no_of_zeroes = num.value.size() - 1;
        remainder.value = abs_dividend.value.substr(abs_dividend.value.size() - no_of_zeroes);
    }
    else {
        BigInteger quotient = abs_dividend / abs_divisor;
        remainder = abs_dividend - quotient * abs_divisor;
    }
    strip_leading_zeroes(remainder.value);

    // remainder has the same sign as that of the dividend
    remainder.sign = this->sign;
    if (remainder.value == "0")     // except if its zero
        remainder.sign = '+';

    return remainder;
}


/*
    BigInt + Integer
    ----------------
*/

BigInteger BigInteger::operator+(const long long& num) const {
    return *this + BigInteger(num);
}


/*
    Integer + BigInt
    ----------------
*/

BigInteger operator+(const long long& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) + rhs;
}


/*
    BigInt - Integer
    ----------------
*/

BigInteger BigInteger::operator-(const long long& num) const {
    return *this - BigInteger(num);
}


/*
    Integer - BigInt
    ----------------
*/

BigInteger operator-(const long long& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) - rhs;
}


/*
    BigInt * Integer
    ----------------
*/

BigInteger BigInteger::operator*(const long long& num) const {
    return *this * BigInteger(num);
}


/*
    Integer * BigInt
    ----------------
*/

BigInteger operator*(const long long& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) * rhs;
}


/*
    BigInt / Integer
    ----------------
*/

BigInteger BigInteger::operator/(const long long& num) const {
    return *this / BigInteger(num);
}


/*
    Integer / BigInt
    ----------------
*/

BigInteger operator/(const long long& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) / rhs;
}


/*
    BigInt % Integer
    ----------------
*/

BigInteger BigInteger::operator%(const long long& num) const {
    return *this % BigInteger(num);
}


/*
    Integer % BigInt
    ----------------
*/

BigInteger operator%(const long long& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) % rhs;
}


/*
    BigInt + String
    ---------------
*/

BigInteger BigInteger::operator+(const std::string& num) const {
    return *this + BigInteger(num);
}


/*
    String + BigInt
    ---------------
*/

BigInteger operator+(const std::string& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) + rhs;
}


/*
    BigInt - String
    ---------------
*/

BigInteger BigInteger::operator-(const std::string& num) const {
    return *this - BigInteger(num);
}


/*
    String - BigInt
    ---------------
*/

BigInteger operator-(const std::string& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) - rhs;
}


/*
    BigInt * String
    ---------------
*/

BigInteger BigInteger::operator*(const std::string& num) const {
    return *this * BigInteger(num);
}


/*
    String * BigInt
    ---------------
*/

BigInteger operator*(const std::string& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) * rhs;
}


/*
    BigInt / String
    ---------------
*/

BigInteger BigInteger::operator/(const std::string& num) const {
    return *this / BigInteger(num);
}


/*
    String / BigInt
    ---------------
*/

BigInteger operator/(const std::string& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) / rhs;
}


/*
    BigInt % String
    ---------------
*/

BigInteger BigInteger::operator%(const std::string& num) const {
    return *this % BigInteger(num);
}


/*
    String % BigInt
    ---------------
*/

BigInteger operator%(const std::string& lhs, const BigInteger& rhs) {
    return BigInteger(lhs) % rhs;
}

#endif  // BIG_INT_BINARY_ARITHMETIC_OPERATORS_HPP


/*
    ===========================================================================
    Arithmetic-assignment operators
    ===========================================================================
*/

#ifndef BIG_INT_ARITHMETIC_ASSIGNMENT_OPERATORS_HPP
#define BIG_INT_ARITHMETIC_ASSIGNMENT_OPERATORS_HPP



/*
    BigInt += BigInt
    ----------------
*/

BigInteger& BigInteger::operator+=(const BigInteger& num) {
    *this = *this + num;

    return *this;
}


/*
    BigInt -= BigInt
    ----------------
*/

BigInteger& BigInteger::operator-=(const BigInteger& num) {
    *this = *this - num;

    return *this;
}


/*
    BigInt *= BigInt
    ----------------
*/

BigInteger& BigInteger::operator*=(const BigInteger& num) {
    *this = *this * num;

    return *this;
}


/*
    BigInt /= BigInt
    ----------------
*/

BigInteger& BigInteger::operator/=(const BigInteger& num) {
    *this = *this / num;

    return *this;
}


/*
    BigInt %= BigInt
    ----------------
*/

BigInteger& BigInteger::operator%=(const BigInteger& num) {
    *this = *this % num;

    return *this;
}


/*
    BigInt += Integer
    -----------------
*/

BigInteger& BigInteger::operator+=(const long long& num) {
    *this = *this + BigInteger(num);

    return *this;
}


/*
    BigInt -= Integer
    -----------------
*/

BigInteger& BigInteger::operator-=(const long long& num) {
    *this = *this - BigInteger(num);

    return *this;
}


/*
    BigInt *= Integer
    -----------------
*/

BigInteger& BigInteger::operator*=(const long long& num) {
    *this = *this * BigInteger(num);

    return *this;
}


/*
    BigInt /= Integer
    -----------------
*/

BigInteger& BigInteger::operator/=(const long long& num) {
    *this = *this / BigInteger(num);

    return *this;
}


/*
    BigInt %= Integer
    -----------------
*/

BigInteger& BigInteger::operator%=(const long long& num) {
    *this = *this % BigInteger(num);

    return *this;
}


/*
    BigInt += String
    ----------------
*/

BigInteger& BigInteger::operator+=(const std::string& num) {
    *this = *this + BigInteger(num);

    return *this;
}


/*
    BigInt -= String
    ----------------
*/

BigInteger& BigInteger::operator-=(const std::string& num) {
    *this = *this - BigInteger(num);

    return *this;
}


/*
    BigInt *= String
    ----------------
*/

BigInteger& BigInteger::operator*=(const std::string& num) {
    *this = *this * BigInteger(num);

    return *this;
}


/*
    BigInt /= String
    ----------------
*/

BigInteger& BigInteger::operator/=(const std::string& num) {
    *this = *this / BigInteger(num);

    return *this;
}


/*
    BigInt %= String
    ----------------
*/

BigInteger& BigInteger::operator%=(const std::string& num) {
    *this = *this % BigInteger(num);

    return *this;
}

#endif  // BIG_INT_ARITHMETIC_ASSIGNMENT_OPERATORS_HPP


/*
    ===========================================================================
    Increment and decrement operators
    ===========================================================================
*/

#ifndef BIG_INT_INCREMENT_DECREMENT_OPERATORS_HPP
#define BIG_INT_INCREMENT_DECREMENT_OPERATORS_HPP



/*
    Pre-increment
    -------------
    ++BigInt
*/

BigInteger& BigInteger::operator++() {
    *this += 1;

    return *this;
}


/*
    Pre-decrement
    -------------
    --BigInt
*/

BigInteger& BigInteger::operator--() {
    *this -= 1;

    return *this;
}


/*
    Post-increment
    --------------
    BigInt++
*/

BigInteger BigInteger::operator++(int) {
    BigInteger temp = *this;
    *this += 1;

    return temp;
}


/*
    Post-decrement
    --------------
    BigInt--
*/

BigInteger BigInteger::operator--(int) {
    BigInteger temp = *this;
    *this -= 1;

    return temp;
}

#endif  // BIG_INT_INCREMENT_DECREMENT_OPERATORS_HPP


/*
    ===========================================================================
    I/O stream operators
    ===========================================================================
*/

#ifndef BIG_INT_IO_STREAM_OPERATORS_HPP
#define BIG_INT_IO_STREAM_OPERATORS_HPP



/*
    BigInt from input stream
    ------------------------
*/

std::istream& operator>>(std::istream& in, BigInteger& num) {
    std::string input;
    in >> input;
    num = BigInteger(input);  // remove sign from value and set sign, if exists

    return in;
}


/*
    BigInt to output stream
    -----------------------
*/

std::ostream& operator<<(std::ostream& out, const BigInteger& num) {
    if (num.sign == '-')
        out << num.sign;
    out << num.value;

    return out;
}

#endif  // BIG_INT_IO_STREAM_OPERATORS_HPP


