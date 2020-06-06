//A minimalistic program multiplying two 64-digit integers.

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <string>

using BigInt = std::vector<char>;

// Init from string. Reversed for convenience.
BigInt fromString(const std::string& str) {
	return BigInt(str.rbegin(), str.rend());
}

// Convert to string. Reversed for convenience.
std::string toString(const BigInt& x) {
	return std::string(x.rbegin(), x.rend());
}

// Add three single-digit numbers.
BigInt addDigits(char x, char y, char z) {
	assert(isdigit(x) && isdigit(y));
	const int sum = (x - '0') + (y - '0') + (z - '0');
	BigInt result;
	result.push_back(sum % 10 + '0');
	if (sum / 10 != 0) {
		result.push_back(sum / 10 + '0');
	}
	return result;
}

// Multiply two single-digit numbers.
BigInt multiplyDigits(char x, char y) {
	assert(isdigit(x) && isdigit(y));
	const int product = (x - '0') * (y - '0');
	BigInt result;
	result.push_back(product % 10 + '0');
	if (product / 10 != 0) {
		result.push_back(product / 10 + '0');
	}
	return result;
}

// Add 2 unsigned BigInts
BigInt addBigInts(const BigInt& x, const BigInt& y) {
	char carry = '0';
	BigInt result;
	for (auto i = 0U, len = std::max(x.size(), y.size()); i < len; i++) {
		const char dx = i >= x.size() ? '0' : x[i];
		const char dy = i >= y.size() ? '0' : y[i];
		const BigInt digitSum = addDigits(dx, dy, carry);
		carry = digitSum.size() > 1 ? digitSum[1] : '0';
		result.push_back(digitSum[0]);
	}
	if (carry != '0') {
		result.push_back(carry);
	}
	return result;
}

// Ad hoc karatsuba multiplication
BigInt karatsuba(const BigInt& x, const BigInt& y) {
	if (x.size() == 1) {
		return multiplyDigits(x[0], y[0]);
	}
	else {
		const size_t halflength = x.size() / 2;
		const BigInt b(x.begin(), x.begin() + halflength);
		const BigInt a(x.begin() + halflength, x.end());
		const BigInt d(y.begin(), y.begin() + halflength);
		const BigInt c(y.begin() + halflength, y.end());
		const BigInt ac = karatsuba(a, c);
		const BigInt bd = karatsuba(b, d);
		// Can be optimized to 3 recursive calls,
		// but I'd need to implement subtraction
		// and handle number lengths that aren't
		// a power of two.
		const BigInt ad = karatsuba(a, d);
		const BigInt bc = karatsuba(b, c);
		const BigInt mid = addBigInts(ad, bc);
		BigInt fullexp(x.size(), '0');
		BigInt halfexp(halflength, '0');
		fullexp.insert(fullexp.end(), ac.begin(), ac.end());
		halfexp.insert(halfexp.end(), mid.begin(), mid.end());
		return addBigInts(addBigInts(fullexp, halfexp), bd);
	}
}

// Some trivial testing to help catch regressions.
void test() {
	{
		assert(toString(fromString("1234")) == std::string("1234"));
	}

	{
		assert(toString(addDigits('1', '9', '9')) == std::string("19"));
	}

	{
		assert(toString(multiplyDigits('9', '7')) == std::string("63"));
	}

	{
		const BigInt x = fromString("1234");
		const BigInt y = fromString("5678");
		assert(std::to_string(1234 + 5678) == toString(addBigInts(x, y)));
	}

	{
		const BigInt x = fromString("1234");
		const BigInt y = fromString("5678");
		assert(std::to_string(1234 * 5678) == toString(karatsuba(x, y)));
	}
}

// Do the assignment.
int main() {
	// test();
	const BigInt x = fromString("31415926535897932384626433832795028841"
		"97169399375105820974944592");
	const BigInt y = fromString("27182818284590452353602874713526624977"
		"57247093699959574966967627");
	std::cout << toString(karatsuba(x, y)) << std::endl;
}