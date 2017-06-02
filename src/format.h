#include <cstdint>
#include <string>

using namespace std;

struct local_file_header {
    uint32_t signature = 0x04034b50;
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
    // file name
    // extra field
};

struct endof_central_directory_record {
    uint32_t signature = 0x06054b50;
    uint16_t num_of_this_disk; //このディスクの番号
    uint16_t num_of_start_central_directory;
    uint16_t total_number_on_this_disk;
    uint16_t total_number;
    uint32_t sizeof_central_directory;
    uint32_t starting_disk_number;
    uint16_t comment_length;
    // comment
};

void create_empty_zip(string path);
