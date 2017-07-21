#include "vector"
#include "string"

class zip {
private:
	vector<zip_entry> entry;
	string comment;
	ofstream wf;

public:
	void add_file(string filename, char *data, size_t size);
}

class zip_entry {

}
