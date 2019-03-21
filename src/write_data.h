/* -*- C++ -*------------------------------------------------------------
 *
 *	write_data.h
 *
 * ---------------------------------------------------------------------- */

#ifndef WRITE_DATA_H
#define WRITE_DATA_H

#include "error.h"
#include "system.h"
#include "stl2lmp.h"

class WriteData
{
public:
	WriteData(Error *error_);
	~WriteData();

	virtual int command(int argc, char * argv[], System &sys);
protected:
	FILE *file;
	char buffer[MAX_STRING];	// buffer char array to store read line
	Error *error;

	bool check_arg(char **arg, const char *flag, int num, int argc);

	virtual int write(char* filename_, int hint_flag, System &sys);
};



#endif // !WRITE_DATA_H
