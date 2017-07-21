#include "format.h"

using namespace std;

int main(int argc, char** argv) {
    string src = argv[1];
    string dist = argv[2];

    create_single_zip(src, dist);
    return 0;
}
