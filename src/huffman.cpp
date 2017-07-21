#include "huffman.h"

std::pair<uint16_t, int> static_huffman_table(uint16_t x) {
	if (x <= 143) {
		return {0b00110000 + x, 8};
	}
	else if (x <= 255) {
		return {0b110010000 + (144 - x), 9};
	}
	else if (x <= 279) {
		return {0b0000000 + (256 - x), 7};
	}
	else {
		return {0b11000000 + (280 - x), 8};
	}
}
