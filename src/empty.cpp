#include "format.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void create_empty_zip(string path) {
    endof_central_directory_record end_cdr;
    end_cdr.num_of_this_disk = 0;
    end_cdr.num_of_start_central_directory = 0;
    end_cdr.total_number_on_this_disk = 0;
    end_cdr.total_number = 0;
    end_cdr.sizeof_central_directory = 0;
    end_cdr.starting_disk_number = 0;
    end_cdr.comment_length = 0;

    ofstream wf;

    wf.open(path, ios::out | ios::binary);
    cout << "Create zip" << endl;
    wf.write((char*)&end_cdr, sizeof(end_cdr));
    wf.close();
}
