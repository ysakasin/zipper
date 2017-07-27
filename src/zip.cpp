#include "format.h"
#include <fstream>
#include <iostream>
#include <string>
#include "bitstream.h"
#include "deflate.h"

using namespace std;

#define SIZE_LOCAL_FILE_HEADER 30

extern uint16_t msdos_time(uint16_t hour, uint16_t min, uint16_t sec);

extern uint16_t msdos_date(uint16_t year, uint16_t month, uint16_t day);

void create_single_zip(string src, string dist) {
    streampos size;
    char * memblock;

    ifstream file (src, ios::in | ios::binary | ios::ate);
    if (file.is_open()) {
        size = file.tellg();
        memblock = new char [size];
        file.seekg (0, ios::beg);
        file.read (memblock, size);
        file.close();

        Bitstream bitstream;
        deflate(memblock, size, &bitstream);

        auto compressed_size = bitstream.byte_size();

        uint32_t crc32 = crc((unsigned char *)memblock, size);
        LocalFileHeader lfh(src, size, compressed_size, crc32);
        CentralDirectoryHeader cdh(src, size, compressed_size, crc32);
        EndofCentralDirectoryRecord eoz(cdh.size(), lfh.size() + compressed_size);

        ofstream wf;

        wf.open(dist, ios::out | ios::binary);
        lfh.dump(wf);
        // wf.write(memblock, size);
        while (!bitstream.empty()) {
			char a = bitstream.fetch();
			wf.write(&a, 1);
		}
        cdh.dump(wf);
        eoz.dump(wf);

        wf.close();

        delete[] memblock;
    }
    else {
        cout << "Unable to open file" << endl;
    }
}
