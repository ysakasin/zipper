#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#define WINDOW_SIZE 0x8000
#define MINIMUM_CHAIN 3
#define MAXIMUM_CHAIN 258

using namespace std;

struct chain {
	int pos;
	int distance;
	int length;
	chain *next;
};

pair<uint16_t, int> static_huffman_table(uint16_t x) {
	if (x <= 143) {
		return {0b00110000 + x, 8}
	}
	else if (x <= 255) {
		return {0b110010000 + (144 - x), 9}
	}
	else if (x <= 279) {
		return {0b0000000 + (256 - x), 7}
	}
	else {
		return {0b11000000 + (280 - x), 8}
	}
}

chain *lz77(char *data, int size, int pos) {
	int begin_pos = (pos < WINDOW_SIZE ? 0 : pos - WINDOW_SIZE);

	int chained_pos = -1;
	int chained_length = MINIMUM_CHAIN - 1;

	for (int i = begin_pos; i < pos - chained_length; i++) {
		int length = 0;
		for (int p = 0; p < pos - i && pos + p < size && length < MAXIMUM_CHAIN; p++) {
			if (data[i + p] == data[pos + p]) {
				length++;
			}
			else {
				break;
			}
		}
		if (length > chained_length) {
			chained_pos = i;
			chained_length = length;
			if (length == MAXIMUM_CHAIN) {
				break;
			}
		}
	}

	if (chained_pos == -1) {
		return NULL;
	}

	chain *res = (chain *)malloc(sizeof(chain));
	res->pos = pos;
	res->distance = pos - chained_pos;
	res->length = chained_length;

	return res;
}

int integer_length(uint16_t x) {
	if (x == 0) {
		return 0;
	}

	int length = 1;
	while ((x = x >> 1) != 0) {
		length++;
	}
	return length;
}

void length_to_code(int length, vector<uint16_t> *codes) {
	if (length <= 10) {
		codes->push_back(254 + length);
		return;
	}
	if (length == 258) {
		codes->push_back(285);
		return;
	}

	int len = integer_length(length) - 3;
	uint32_t divisor = powl(2, len);
	uint32_t base = (length-3) / divisor;
	uint32_t extra = (length-3) % divisor;

	codes->push_back(257 + (4 * len) + base);
	codes->push_back(extra);
}

void distance_to_code(int length, vector<uint16_t> *codes) {
	if (length <= 2) {
		codes->push_back(length - 1);
		return;
	}

	int len = integer_length(length) - 2;
	uint32_t divisor = powl(2, len);
	uint32_t base = (length-1) / divisor;
	uint32_t extra = (length-1) % divisor;

	codes->push_back(2 * len + base);
	codes->push_back(extra);
}

void chain_to_codes(char *data, int size, chain *chain_p, vector<uint16_t> *codes) {
	int pos = 0;
	while (pos < size) {
		if (chain_p == NULL) {
			codes->push_back(data[pos]);
			pos++;
		}
		else if (chain_p->pos == pos) {
			length_to_code(chain_p->length, codes);
			distance_to_code(chain_p->distance, codes);
			pos += chain_p->length;
			chain_p = chain_p->next;
		} else {
			codes->push_back(data[pos]);
			pos++;
		}
	}
	codes->push_back(256);
}

int main() {
	ifstream file ("LICENSE", ios::in | ios::binary | ios::ate);
	if (!file.is_open()) {
		cout << "not open" << endl;
		return 0;
	}
	auto size = file.tellg();
    char *text = new char [size];
    file.seekg (0, ios::beg);
    file.read (text, size);
    file.close();

	int pos = 0;
	chain *chain_head;
	chain *chain_tail = NULL;

	while (pos < size) {
		chain *res = lz77(text, size, pos);
		if (res == NULL) {
			cout << text[pos];
			pos++;
		}
		else {
			cout << "<" << res->distance << "," << res->length << ">";
			pos += res->length;
			if (chain_tail == NULL) {
				chain_head = chain_tail = res;
			}
			else {
				chain_tail->next = res;
				chain_tail = res;
			}
		}
	}
	cout << endl;

	vector<uint16_t> codes;
	chain_to_codes(text, size, chain_head, &codes);

	for (auto n : codes) {
		cout << n << " ";
	}
	cout << endl;

	Bitstream bitstream;

	int i = 0;
	while (i < codes.size()) {
		if (codes[i] >= 257) {
			bitstream.add(static_huffman_table(codes[i]));
			bitstream.add(codes[i+1], length)
			bitstream.add(codes[i+2], 5);
		}
		else {
			bitstream.add(static_huffman_table(codes[i]));
			i++;
		}
	}
}
