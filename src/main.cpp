#include "zip.h"
#include "format.h"

using namespace std;

int main(int argc, char** argv) {
	string src = argv[1];
	string dist = argv[2];

	Zip zip(dist);
	zip.add(src);
	zip.finalize();
	return 0;
}
