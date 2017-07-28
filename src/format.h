#pragma once

#include <cstdint>
#include <string>
#include <fstream>

class LocalFileHeader {
private:
	const uint32_t signature = 0x04034b50;
	uint16_t version;
	uint16_t flag;
	uint16_t compression_method;
	uint16_t mod_file_time;
	uint16_t mod_file_date;
	uint32_t crc32;
	uint32_t compressed_size;
	uint32_t uncompressed_size;
	uint16_t file_name_length;
	uint16_t extra_field_length;
	std::string filename;

public:
	LocalFileHeader(std::string filename, uint32_t size, uint32_t compressed_size, uint32_t crc32);
	void dump(std::ofstream& stream);
	uint32_t size() { return 30 + filename.size(); }
	void set_dir_flag() { version = 10; compression_method = 0; }
};

class CentralDirectoryHeader {
private:
	const uint32_t signature = 0x02014b50;
	uint16_t version;
	uint16_t version_needed_to_extract;
	uint16_t flag;
	uint16_t compression_method;
	uint16_t mod_file_time;
	uint16_t mod_file_date;
	uint32_t crc32;
	uint32_t compressed_size;
	uint32_t uncompressed_size;
	uint16_t file_name_length;
	uint16_t extra_field_length;
	uint16_t file_comment_length;
	uint16_t disk_number_start;
	uint16_t internal_file_attributes;
	uint32_t external_file_attributes;
	uint32_t offset_of_local_header;
	std::string filename;

public:
	CentralDirectoryHeader(std::string filename, uint32_t size, uint32_t compressed_size, uint32_t crc32, uint32_t offset);
	void dump(std::ofstream& stream);
	uint32_t size() { return 46 + filename.size(); }
	void set_dir_flag() { flag |= 1UL << 4; }
};

class EndofCentralDirectoryRecord {
private:
	const uint32_t signature = 0x06054b50;
	uint16_t num_of_this_disk; //このディスクの番号
	uint16_t num_of_start_central_directory;
	uint16_t total_number_on_this_disk;
	uint16_t total_number;
	uint32_t sizeof_central_directory;
	uint32_t starting_disk_number; //offset
	uint16_t comment_length;

public:
	EndofCentralDirectoryRecord(uint32_t size, uint32_t offset, uint16_t nrecord);
	void dump(std::ofstream& stream);
};
