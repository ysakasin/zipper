class Bitstream {
private:
	queue<int> stream;
public:
	char fetch() {
		char res = 0x00;
		for (int i = 0; i < 8 && !stream.empty(); i++) {
			res = res | (stream.front() << i);
			stream.pop();
		}
		return res;
	}
	void add(uint16_t n, int size) {
		while (size > 0) {
			int x = (n & (0x01 << --size)) ? 1 : 0;
			stream.push(x);
		}
	}

	void add(pair<uint16_t, int> p) {
		add(p.first, p.second);
	}
}
