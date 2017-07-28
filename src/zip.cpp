#include "zip.h"
#include "format.h"
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <string>
#include "bitstream.h"
#include "deflate.h"

using namespace std;

#define SIZE_LOCAL_FILE_HEADER 30

extern uint16_t msdos_time(uint16_t hour, uint16_t min, uint16_t sec);

extern uint16_t msdos_date(uint16_t year, uint16_t month, uint16_t day);

Zip::Zip(string dist) {
	wf.open(dist, ios::out | ios::binary);

	if (!wf.is_open()) {
		throw "dist file can not open";
	}
}

void Zip::add(string path) {
	cout << path << endl;
	struct stat status;
	if (stat(path.c_str(), &status) != 0) {
		cout << "stat error" << endl;
		throw "stat error";
	}

	if (S_ISREG(status.st_mode)) {
		add_file(path);
		return;
	}

	if (!S_ISDIR(status.st_mode)) {
		cout << "unexpected obj" << endl;
		throw "unexpected obj";
	}

	DIR *dir = opendir(path.c_str());
	if (dir == NULL) {
		cout << "can not open dir" << endl;
		throw "can not open dir";
	}
	struct dirent *ent;
	int num_child = 0;
	while ((ent = readdir(dir)) != NULL) {
		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
			continue;
		}
		add(path + "/" + ent->d_name);
		num_child++;
	}

	if (num_child == 0) {
		add_dir(path);
	}
}

void Zip::add_file(string filename) {
	ifstream file (filename, ios::in | ios::binary | ios::ate);
	if (!file.is_open()) {
		throw "src file can not open";
	}

	auto size = file.tellg();
	char *memblock = new char [size];
	file.seekg (0, ios::beg);
	file.read (memblock, size);
	file.close();

	add_file(filename, memblock, size);

	delete[] memblock;
}

void Zip::add_file(string filename, char *data, size_t org_size) {
	Bitstream bitstream;
	deflate(data, org_size, &bitstream);

	uint32_t crc32 = crc((unsigned char *)data, org_size);
	uint32_t compressed_size = bitstream.byte_size();

	LocalFileHeader lfh(filename, org_size, compressed_size, crc32);
	lfh.dump(wf);

	wf.write(bitstream.data(), compressed_size);

	CentralDirectoryHeader cdh(filename, org_size, compressed_size, crc32, offset);
	records.push_back(cdh);

	offset += lfh.size() + compressed_size;
	central_dir_size += cdh.size();
}

void Zip::add_dir(string dirname) {
	if (dirname.back() != '/') {
		dirname.push_back('/');
	}

	LocalFileHeader lfh(dirname, 0, 0, 0);
	lfh.set_dir_flag();
	lfh.dump(wf);

	CentralDirectoryHeader cdh(dirname, 0, 0, 0, offset);
	cdh.set_dir_flag();
	records.push_back(cdh);

	offset += lfh.size();
	central_dir_size += cdh.size();
}

void Zip::finalize() {
	for (auto cdh : records) {
		cdh.dump(wf);
	}

	EndofCentralDirectoryRecord eoz(central_dir_size, offset, records.size());
	eoz.dump(wf);

	wf.close();
}
