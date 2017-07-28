#include "huffman.h"
#include <iostream>
using namespace std;

std::pair<uint16_t, int> static_huffman_table(uint16_t x) {
	if (x <= 143) {
		return {0b00110000 + x, 8};
	}
	else if (x <= 255) {
		return {0b110010000 + (x - 144), 9};
	}
	else if (x <= 279) {
		return {0b0000000 + (x - 256), 7};
	}
	else if (x <= 287) {
		return {0b11000000 + (x - 280), 8};
	}
	else {
		cout << "huffman out of range" << endl;
		throw "huffman out of range";
	}
}
