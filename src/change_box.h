/* -*- C++ -*------------------------------------------------------------
 *
 *	change_box.h
 *
 * ---------------------------------------------------------------------- */

#ifndef CHANGEBOX_H
#define CHANGEBOX_H

#include "error.h"
#include "system.h"
#include "stl2lmp.h"

class ChangeBox
{
public:
	ChangeBox(Error * error_);
	~ChangeBox();

	virtual int command(int argc, char * argv[], System & sys);
protected:
	Error *error;

	bool check_arg(char **arg, const char *flag, int num, int argc);
	
	int periodic_to_shrinkwarpped(System & sys, int xyz[3]);
private:



};

#endif // !CHANGEBOX_H

