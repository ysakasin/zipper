#include "format.h"

uint16_t msdos_time(uint16_t hour, uint16_t min, uint16_t sec) {
	return hour << 11 | min << 5 | (sec / 2);
}

uint16_t msdos_date(uint16_t year, uint16_t month, uint16_t day) {
	return year << 11 | month << 5 | day;
}

LocalFileHeader::LocalFileHeader(string filename, uint32_t size, uint32_t crc32) {
	version = 10;
	flag = 0;
	compression_method = 0;
	mod_file_time = msdos_time(12, 0, 0);
	mod_file_date = msdos_date(2017, 7, 7);
	this->crc32 = crc32;
	compressed_size = size;
	uncompressed_size = size;
	file_name_length = filename.size();
	extra_field_length = 0;
	this->filename = filename;
}

void LocalFileHeader::dump(ofstream& wf) {
	wf.write((char *) &signature, sizeof(signature));
	wf.write((char *) &version, sizeof(version));
	wf.write((char *) &flag, sizeof(flag));
	wf.write((char *) &compression_method, sizeof(compression_method));
	wf.write((char *) &mod_file_time, sizeof(mod_file_time));
	wf.write((char *) &mod_file_date, sizeof(mod_file_date));
	wf.write((char *) &crc32, sizeof(crc32));
	wf.write((char *) &compressed_size, sizeof(compressed_size));
	wf.write((char *) &uncompressed_size, sizeof(uncompressed_size));
	wf.write((char *) &file_name_length, sizeof(file_name_length));
	wf.write((char *) &extra_field_length, sizeof(extra_field_length));
	wf.write(filename.c_str(), file_name_length);
}

CentralDirectoryHeader::CentralDirectoryHeader(string filename, uint32_t size, uint32_t crc32) {
	version = 0x3f00; // ZIP v6.3
	version_needed_to_extract = 10;
	flag = 0;
	compression_method = 0;
	mod_file_time = msdos_time(12, 0, 0);
	mod_file_date = msdos_date(2017, 7, 7);
	this->crc32 = crc32;
	compressed_size = size;
	uncompressed_size = size;
	file_name_length = filename.size();
	extra_field_length = 0;
	file_comment_length = 0;
	disk_number_start = 0;
	internal_file_attributes = 0;
	external_file_attributes = 0;
	offset_of_local_header = 0;
	this->filename = filename;
}

void CentralDirectoryHeader::dump(ofstream& wf) {
	wf.write((char *) &signature, sizeof(signature));
	wf.write((char *) &version, sizeof(version));
	wf.write((char *) &version_needed_to_extract, sizeof(version_needed_to_extract));
	wf.write((char *) &flag, sizeof(flag));
	wf.write((char *) &compression_method, sizeof(compression_method));
	wf.write((char *) &mod_file_time, sizeof(mod_file_time));
	wf.write((char *) &mod_file_date, sizeof(mod_file_date));
	wf.write((char *) &crc32, sizeof(crc32));
	wf.write((char *) &compressed_size, sizeof(compressed_size));
	wf.write((char *) &uncompressed_size, sizeof(uncompressed_size));
	wf.write((char *) &file_name_length, sizeof(file_name_length));
	wf.write((char *) &extra_field_length, sizeof(extra_field_length));
	wf.write((char *) &file_comment_length, sizeof(file_comment_length));
	wf.write((char *) &disk_number_start, sizeof(disk_number_start));
	wf.write((char *) &internal_file_attributes, sizeof(internal_file_attributes));
	wf.write((char *) &external_file_attributes, sizeof(external_file_attributes));
	wf.write((char *) &offset_of_local_header, sizeof(offset_of_local_header));
	wf.write(filename.c_str(), file_name_length);
}

EndofCentralDirectoryRecord::EndofCentralDirectoryRecord(uint32_t size, uint32_t offset) {
	num_of_this_disk = 0;
    num_of_start_central_directory = 0;
    total_number_on_this_disk = 1;
    total_number = 1;
    sizeof_central_directory = size;
    starting_disk_number = offset;
    comment_length = 0;
}

void EndofCentralDirectoryRecord::dump(ofstream& wf) {
	wf.write((char *) &signature, sizeof(signature));
	wf.write((char *) &num_of_this_disk, sizeof(num_of_this_disk));
	wf.write((char *) &num_of_start_central_directory, sizeof(num_of_start_central_directory));
	wf.write((char *) &total_number_on_this_disk, sizeof(total_number_on_this_disk));
	wf.write((char *) &total_number, sizeof(total_number));
	wf.write((char *) &sizeof_central_directory, sizeof(sizeof_central_directory));
	wf.write((char *) &starting_disk_number, sizeof(starting_disk_number));
	wf.write((char *) &comment_length, sizeof(comment_length));
}
