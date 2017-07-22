#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "./bitstream.h"

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

void length_to_bitstream(int length, Bitstream *bstream) {
	if (length <= 10) {
		// cout << 254 + length << endl;
		bstream->push_with_encode(254 + length);
		return;
	}
	if (length == 258) {
		bstream->push_with_encode(285);
		return;
	}

	int n = length;
	int n3 = n - 3;
	int len = integer_length(n3);
	int len3 = len - 3;

	uint32_t divisor = powl(2, len3);
	uint32_t base = n3 / divisor;
	uint32_t extra = n3 % divisor;

	bstream->push_with_encode(257 + (4 * len3) + base);
	bstream->push_extra(extra, len3);
}

void distance_to_bitstream(int length, Bitstream *bstream) {
	if (length <= 4) {
		bstream->push(length - 1, 5);
		return;
	}

	int n = length;
	int n1 = n - 1;
	int len = integer_length(n1);
	int len2 = len - 2;

	uint32_t divisor = powl(2, len2);
	uint32_t base = (n1) / divisor;
	uint32_t extra = (n1) % divisor;

	bstream->push(2 * len2 + base, 5);
	bstream->push_extra(extra, len2);
}

void chain_to_bitstream(char *data, int size, chain *chain_p, Bitstream *bstream) {
	int pos = 0;
	// static int i = 1;
	while (pos < size) {
		if (chain_p == NULL) {
			bstream->push_with_encode(data[pos]);
			pos++;
		}
		else if (chain_p->pos == pos) {
			cout << chain_p->length << endl;
			length_to_bitstream(chain_p->length, bstream);
			distance_to_bitstream(chain_p->distance, bstream);
			pos += chain_p->length;
			chain_p = chain_p->next;
			// if (i < 4) {
			// 	i++;
			// }
			// else {
			// 	chain_p = NULL;
			// }
		} else {
			bstream->push_with_encode(data[pos]);
			pos++;
		}
	}
	bstream->push_with_encode(256);
}

unsigned long crc(unsigned char *buf, int len);

int main() {
	for (int i = 3; i <= 258; i++) {

	}
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
			cout << "<" << res->pos << "," << res->length << "," << res->distance << ">";
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

	Bitstream bitstream;
	bitstream.push(0b1,  1);
	bitstream.push(0b10, 2);

	ofstream wf;
	// wf.open("tmp/test.gz", ios::out | ios::binary);


	// while (!bitstream.empty()) {
 //    	char a = bitstream.fetch();
	// 	wf.write(&a, 1);
	// }
	// return 0;
	chain_to_bitstream(text, size, chain_head, &bitstream);


    wf.open("tmp/test.gz", ios::out | ios::binary);

	/* gzip header */
	unsigned char signature[2] = {0x1f, 0x8b};
	wf.write((char *)signature, sizeof(char) * 2);

	char method = 8;
	wf.write(&method, sizeof(char));

	char flag = 0b00001000;
	wf.write(&flag, sizeof(char));

	uint32_t mtime = 0;
	wf.write((char *)&mtime, sizeof(uint32_t));

	char extra_flag = 0;
	wf.write(&extra_flag, sizeof(char));

	char os = 0xff;
	wf.write(&os, sizeof(char));

	wf.write("test.txt", sizeof(char) * 9);

	// char xlen = 0;
	// wf.write(&xlen, sizeof(char));
	/* gzip header end */

    while (!bitstream.empty()) {
    	char a = bitstream.fetch();
		wf.write(&a, 1);
	}

	uint32_t crc32 = crc((unsigned char *)text, size);
	uint32_t isize = size % 0x100000000;
	wf.write((char *)&crc32, sizeof(uint32_t));
	wf.write((char *)&isize, sizeof(uint32_t));

    wf.close();

	return 0;
}
