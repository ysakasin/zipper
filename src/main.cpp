#include "format.h"

using namespace std;

int main(int argc, char** argv) {
    string filepath = argv[1];
    create_empty_zip(filepath);
    return 0;
}
