/* -*- C++ -*------------------------------------------------------------
 *
 *	read_data.h
 *
 * ---------------------------------------------------------------------- */

#ifndef READ_DATA_H
#define READ_DATA_H

#include "error.h"
#include "system.h"
#include "stl2lmp.h"

class ReadData
{
public:
	ReadData(Error *error_);
	~ReadData();	
	
	virtual int command(int argc, char * argv[], System &sys);
protected:
	FILE* file;
	char buffer[MAX_STRING];	// buffer char array to store read line
	char header[MAX_STRING];	// header
	Error *error;

	bool check_arg(char **arg, const char *flag, int num, int argc);
	int read_section(FILE* file_, int nrows_, int ncolumns_, char *buffer_);
	int count_words(const char* line);

	virtual int over(char* filename_, System &sys);
private:
	
};


#endif // !READ_DATA_H

