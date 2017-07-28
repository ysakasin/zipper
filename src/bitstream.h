#pragma once

#include <cstdint>
#include <queue>
#include <vector>
#include "huffman.h"

class Bitstream {
private:
	std::queue<char> stream;
	std::vector<char> v;

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

		while (stream.size() >= 8) {
			char x = fetch();
			v.push_back(x);
		}
	}

	void push_extra(uint16_t n, int size) {
		for (int i = 0; i < size; i++) {
			char x = (n & (0x01 << i)) ? 1 : 0;
			stream.push(x);
		}
	}

	void push(std::pair<uint16_t, int> p) {
		push(p.first, p.second);
	}

	void push_with_encode(uint16_t n) {
		push(static_huffman_table(n));
	}

	void finalize() {
		while (stream.size() > 0) {
			char x = fetch();
			v.push_back(x);
		}
	}

	size_t byte_size() {
		return v.size();
	}

	char *data() {
		return v.data();
	}
};
