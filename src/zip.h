#pragma once

#include <list>
#include <string>
#include "format.h"

class Zip {
private:
	std::list<CentralDirectoryHeader> records;
	std::string comment = "";
	std::ofstream wf;
	size_t offset = 0;
	size_t central_dir_size = 0;

public:
	Zip(std::string dist);
	void add(std::string path);
	void add_file(std::string filename);
	void add_file(std::string filename, char *data, size_t size);
	void add_dir(std::string dirname);
	void finalize();
};

class ZipEntry {
private:
	std::string filename;
	uint32_t org_size;
	uint32_t compressed_size;
	uint32_t crc32;
	uint32_t offset_file_header;

public:
	void dump_local_file_header(std::ofstream& stream);
	void dump_central_directory_header(std::ofstream& stream);
};
