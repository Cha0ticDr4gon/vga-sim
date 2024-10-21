#include <iostream>
#include <string>
#include <cstdint>
#include <cstring>

//Class template up here because C++
template <int I, int D>
class Fixed {
public:
	unsigned char data[(I + D - 1) / 8 + 1];
};

//Prototypes because stupid C++
template <int I, int D>
unsigned char add(Fixed<I, D>& dest, Fixed<I, D>& a, Fixed<I, D>& b, unsigned char carry);
template <int I, int D>
unsigned char add(Fixed<I, D>& dest, Fixed<I, D>& a, Fixed<I, D>& b);
template <int I, int D>
unsigned char sub(Fixed<I, D>& dest, Fixed<I, D>& a, Fixed<I, D>& b);
template <int I, int D>
void inv(Fixed<I, D>& dest, Fixed<I, D>& a);
template <int I, int D>
void shl(Fixed<I, D>& dest, Fixed<I, D>& a, unsigned int n);
template <int I, int D>
void shr(Fixed<I, D>& dest, Fixed<I, D>& a, unsigned int n);
template <int I, int D>
void mul(Fixed<I * 2, D * 2>& dest, Fixed<I, D>& a, Fixed<I, D>& b);
template <int I, int D>
void mul(Fixed<I, D>& dest, Fixed<I, D>& a, Fixed<I, D>& b);
template <int I, int D>
void div(Fixed<I * 2, D * 2>& dest, Fixed<I, D>& a, Fixed<I, D>& b);
template <int I, int D>
void div(Fixed<I, D>& dest, Fixed<I, D>& a, Fixed<I, D>& b);
template <int I, int D>
void copyd(Fixed<2 * I, 2 * D>& dest, Fixed<I, D>& a);
template <int I, int D>
void copyh(Fixed<I, D>& dest, Fixed<I * 2, D * 2>& a);
template <int I, int D>
void print(Fixed<I, D>& a, const std::string& msg);

template <int I, int D>
unsigned char add(Fixed<I, D>& dest, Fixed<I, D>& a, Fixed<I, D>& b, unsigned char carry) {
	const unsigned int num_bytes = (I + D - 1) / 8 + 1;
	Fixed<I, D> temp;
	//Maybe I'll do it with 64-bit numbers in the future
	for(int i = 0; i < num_bytes; i++) {
		temp.data[i] = a.data[i] + b.data[i] + carry;
		carry = ((unsigned char) 0xFF - a.data[i]) < (b.data[i] + carry);
	}
	//Cut off end of data
	temp.data[num_bytes - 1] &= (unsigned char) 0xFF >> ((I + D) % 8);
	std::memcpy(dest.data, temp.data, num_bytes);
	return carry;
}

template <int I, int D>
unsigned char add(Fixed<I, D>& dest, Fixed<I, D>& a, Fixed<I, D>& b) {
	return add(dest, a, b, 0);
}

template <int I, int D>
unsigned char sub(Fixed<I, D>& dest, Fixed<I, D>& a, Fixed<I, D>& b) {
	const unsigned int num_bytes = (I + D - 1) / 8 + 1;
	Fixed<I, D> b_inv;
	//Invert bytes of b
	for(int i = 0; i < num_bytes; i++) b_inv.data[i] = ~b.data[i];
	//Truncate to length
	b_inv.data[num_bytes - 1] &= (unsigned char) 0xFF >> ((I + D) % 8);
	//Add with carry set for sub
	return add(dest, a, b_inv, 1);
}

template <int I, int D>
void inv(Fixed<I, D>& dest, Fixed<I, D>& a) {
	const unsigned int num_bytes = (I + D - 1) / 8 + 1;
	Fixed<I, D> temp;
	for(int i = 0; i < num_bytes; i++) temp.data[i] = ~a.data[i];
	temp.data[num_bytes - 1] &= (unsigned char) 0xFF >> ((I + D) % 8);
	std::memcpy(dest.data, temp.data, num_bytes);
}

template <int I, int D>
void shl(Fixed<I, D>& dest, Fixed<I, D>& a, unsigned int n) {
	const unsigned int num_bytes = (I + D - 1) / 8 + 1;
	Fixed<I, D> temp;
	int byte_offset = n / 8;
	int bit_offset = n % 8;
	//Clear destination bits
	std::memset(temp.data, 0, num_bytes);
	//Shift in bits
	for(int i = 0; i < num_bytes - byte_offset; i++) {
		unsigned char lower = a.data[i] << bit_offset;
		unsigned char upper = a.data[i] >> (8 - bit_offset);
		//I'm lazy rn
		if(i + byte_offset < num_bytes) temp.data[i + byte_offset] |= lower;
		if(i + 1 + byte_offset < num_bytes) temp.data[i + 1 + byte_offset] |= upper;
	}
	//Cut off end of data
	temp.data[num_bytes - 1] &= (unsigned char) 0xFF >> ((I + D) % 8);
	std::memcpy(dest.data, temp.data, num_bytes);
}

template <int I, int D>
void shr(Fixed<I, D>& dest, Fixed<I, D>& a, unsigned int n) {
	const unsigned int num_bytes = (I + D - 1) / 8 + 1;
	Fixed<I, D> temp;
	//These were unsigned, which caused problems with implicit type casting
	int byte_offset = n / 8;
	int bit_offset = n % 8;
	//Clear destination bits
	std::memset(temp.data, 0, num_bytes);
	//Shift in bits
	for(int i = num_bytes - 1; i >= byte_offset; i--) {
		unsigned char lower = a.data[i] << (8 - bit_offset);
		unsigned char upper = a.data[i] >> bit_offset;
		//Lazy
		if(i - byte_offset >= 0) temp.data[i - byte_offset] |= upper;
		if(i - 1 - byte_offset >= 0) temp.data[i - 1 - byte_offset] |= lower;
	}
	//Cut off end of data
	temp.data[num_bytes - 1] &= (unsigned char) 0xFF >> ((I + D) % 8);
	std::memcpy(dest.data, temp.data, num_bytes);
}

template <int I, int D>
void mul(Fixed<I * 2, D * 2>& dest, Fixed<I, D>& a, Fixed<I, D>& b) {
	const unsigned int num_bytes = (2 * (I + D) - 1) / 8 + 1;
	const unsigned int num_bits = I + D;
	//Copy to double-wide numbers to keep precision
	Fixed<I * 2, D * 2> a_wide;
	Fixed<I * 2, D * 2> b_wide;
	Fixed<I * 2, D * 2> product;
	copyd(a_wide, a);
	copyd(b_wide, b);
	std::memset(product.data, 0, num_bytes);
	//Shift-add multiplication
	for(int i = 0; i < num_bits; i++) {
		int bit_offset = i % 8;
		int byte_offset = i / 8;
		Fixed<I * 2, D * 2> shifted;
		shl<I * 2, D * 2>(shifted, b_wide, i);
		if((a.data[byte_offset] >> bit_offset) & 0x01) add<I * 2, D * 2>(product, product, shifted);
	}
	//Shift back to correct precision
	shr<I * 2, D * 2>(product, product, D);
	std::memcpy(dest.data, product.data, num_bytes);
}

//Same precision result as input variables
template <int I, int D>
void mul(Fixed<I, D>& dest, Fixed<I, D>& a, Fixed<I, D>& b) {
	Fixed<I * 2, D * 2> temp;
	mul(temp, a, b);
	copyh(dest, temp);
}

template <int I, int D>
void div(Fixed<I * 2, D * 2>& dest, Fixed<I, D>& a, Fixed<I, D>& b) {
	const unsigned int num_bytes = (I + D - 1) / 8 + 1;
	const unsigned int num_bytes_d = (2 * (I + D) - 1) / 8 + 1;
	const unsigned int num_bits = I + D;
	//Copy to double-wide numbers to keep precision
	Fixed<I * 2, D * 2> a_wide;
	Fixed<I * 2, D * 2> b_wide;
	Fixed<I * 2, D * 2> quotient;
	copyd<I, D>(a_wide, a);
	//copyd<I, D>(b_wide, b);
	std::memset(b_wide.data, 0, num_bytes_d);
	std::memcpy(b_wide.data, b.data, num_bytes);
	//print<I * 2, D * 2>(a_wide, "a_wide");
	//print<I * 2, D * 2>(b_wide, "b_wide");
	std::memset(quotient.data, 0, num_bytes_d);
	//Shift-sub division
	for(int i = num_bits - 1; i >= 0; i--) {
		Fixed<I * 2, D * 2> shifted;
		Fixed<I * 2, D * 2> diff;
		shl<I * 2, D * 2>(shifted, b_wide, i);
		//print<I * 2, D * 2>(shifted, "shiftd");
		//Add bit if difference is still positive
		if(sub<I * 2, D * 2>(diff, a_wide, shifted)) {
			std::memcpy(a_wide.data, diff.data, num_bytes_d);
			quotient.data[i / 8] |= 0x01 << (i % 8);
		}
		//print<I * 2, D * 2>(a_wide, "a_wide");
		//print<I * 2, D * 2>(quotient, "q");
	}
	shl<I * 2, D * 2>(dest, quotient, D);
	//print<I * 2, I * 2>(dest, "dest");
}

template <int I, int D>
void div(Fixed<I, D>& dest, Fixed<I, D>& a, Fixed<I, D>& b) {
	Fixed<I * 2, D * 2> temp;
	div(temp, a, b);
	copyh(dest, temp);
}

template <int I, int D>
void copyd(Fixed<2 * I, 2 * D>& dest, Fixed<I, D>& a) {
	const unsigned int num_bytes = (I + D - 1) / 8 + 1;
	const unsigned int num_bytes_d = (2 * (I + D) - 1) / 8 + 1;
	std::memset(dest.data, 0, num_bytes_d);
	std::memcpy(dest.data, a.data, num_bytes);
	shl<2 * I, 2 * D>(dest, dest, D);
}

template <int I, int D>
void copyh(Fixed<I, D>& dest, Fixed<I * 2, D * 2>& a) {
	const unsigned int num_bytes = (I + D - 1) / 8 + 1;
	Fixed<I * 2, D * 2> temp;
	shr<I * 2, D * 2>(temp, a, D);
	std::memcpy(dest.data, temp.data, num_bytes);
	dest.data[num_bytes - 1] &= (unsigned char) 0xFF >> ((I + D) % 8);
}

template <int I, int D>
void print(Fixed<I, D>& a, const std::string& msg) {
	std::cout << msg << " = ";
	for(int i = I + D - 1; i >= 0; i--) {
		std::cout << (a.data[i / 8] >> (i % 8) & (unsigned char) 0x01);
		if(i == D) std::cout << ".";
	}
	std::cout << std::endl;
}

//TODO: Implement this using double dabble algorithm
template<int I, int D>
void printd(Fixed<I, D>& a, const std::string& msg) {
	std::cout << msg << " = ";
	std::cout << "TBD" << std::endl;
}

int main() {
	Fixed<8, 8> skibidi;
	skibidi.data[0] = 0x81;
	skibidi.data[1] = 0x07;
	print<8, 8>(skibidi, "skibidi");

	Fixed<8, 8> rizzler;
	rizzler.data[0] = 0x80;
	rizzler.data[1] = 0x00;
	print<8, 8>(rizzler, "rizzler");

	Fixed<8, 8> sum;
	add<8, 8>(sum, skibidi, rizzler);
	print<8, 8>(sum, "sum");

	Fixed<8, 8> diff;
	sub<8, 8>(diff, skibidi, rizzler);
	print<8, 8>(diff, "diff");

	//Test shifting left
	std::cout << "Shift left:" << std::endl;
	std::cout << "===========" << std::endl;
	for(int i = 0; i < 10; i++) {
		Fixed<8, 8> shift;
		shl<8, 8>(shift, skibidi, i);
		print<8, 8>(shift, "shift");
	}

	//Test shifting right
	std::cout << "Shift right:" << std::endl;
	std::cout << "============" << std::endl;
	for(int i = 0; i < 10; i++) {
		Fixed<8, 8> shift;
		shr<8, 8>(shift, skibidi, i);
		print<8, 8>(shift, "shift");
	}

	//Two
	Fixed<8, 8> two;
	two.data[0] = 0x00;
	two.data[1] = 0x03;

	//Test multiplication
	std::cout << "Multplication:" << std::endl;
	std::cout << "==============" << std::endl;
	Fixed<8, 8> product;
	mul<8, 8>(product, skibidi, rizzler);
	print<8, 8>(skibidi, "skibidi");
	print<8, 8>(rizzler, "rizzler");
	print<8, 8>(product, "product");
	mul<8, 8>(product, skibidi, two);
	print<8, 8>(product, "product");

	//Test division
	std::cout << "Division:" << std::endl;
	std::cout << "==============" << std::endl;
	Fixed<8, 8> quotient;
	div<8, 8>(quotient, skibidi, rizzler);
	print<8, 8>(skibidi, "skibidi");
	print<8, 8>(rizzler, "rizzler");
	print<8, 8>(quotient, "quotient");
	div<8, 8>(quotient, skibidi, two);
	print<8, 8>(quotient, "quotient");
}
