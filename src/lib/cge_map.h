#ifndef _H_CGE_MAP_
#define _H_CGE_MAP_

#include <fstream>
#include "cge.h"

class CGE;

class cge_map {

	int width, height;
	char *bytes = NULL;
	typedef struct {char f, b;} color;
	color *color_table = NULL;
	bool exists, valid;
	std::ifstream file;


	friend CGE;

public:

	cge_map(const char* file_name);
	void load(void);
	~cge_map(void);

};

#endif