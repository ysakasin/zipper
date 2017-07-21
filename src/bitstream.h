#include <cstdint>
#include <queue>
#include "huffman.h"

class Bitstream {
private:
	std::queue<char> stream;

public:
	bool empty() {
		return stream.empty();
	}

	char fetch() {
		char res = 0x00;
		for (int i = 0; i < 8 && !stream.empty(); i++) {
			res = res | (stream.front() << i);
			stream.pop();
		}
		return res;
	}
	void push(uint16_t n, int size) {
		while (size > 0) {
			char x = (n & (0x01 << --size)) ? 1 : 0;
			stream.push(x);
		}
	}

	// void push(uint16_t n, int size) {
	// 	for (int i = 0; i < size; i++) {
	// 		char x = (n & (0x01 << i)) ? 1 : 0;
	// 		std::cout << (int)x;
	// 		stream.push(x);
	// 	}
	// }

	void push(std::pair<uint16_t, int> p) {
		push(p.first, p.second);
	}

	void push_with_encode(uint16_t n) {
		push(static_huffman_table(n));
	}
};
