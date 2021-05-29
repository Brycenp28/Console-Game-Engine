#include "cge_map.h"

cge_map::cge_map(const char* file_name)
{
	file.open(file_name);
	exists = file.is_open();
	valid = (file >> height && file >> width);

	if (valid) {
		bytes = new char[width * height];
		color_table = new color[width * height];
	}
}

void cge_map::load(void)
{
	if (!exists)
		throw "Could not open the file.";
	if (!valid)
		throw "The file is not valid.";

	char data;

	for (int i = 0; i < width * height; i++) {

		if (!file >> data)
			throw "File corruption.";

		file >> bytes[i];
		file >> color_table[i].f;
		file >> color_table[i].b;

	};
}

cge_map::~cge_map()
{	
	delete [] bytes;
	delete [] color_table;
	file.close();
}