/* -*- C++ -*------------------------------------------------------------
 *
 *	error.h
 *
 * ---------------------------------------------------------------------- */

#ifndef ERROR_H
#define ERROR_H

#include "stl2lmp.h"

class Error
{
	enum Mode { silent, basic, verbose };
	enum Level { ignore, stop };
public:
	Error();
	~Error();

	void setMode(int mode_);
	void setLevel(int level_);
	int warning(const char * warning_, int warncode_);
	int warning(const char* warning_, int warncode_, const char* str);
	int message(const char * message_, int exitcode_);
	int message(const char* message_, int exitcode_, const char* str);
protected:
	Mode mode;
	Level level;
private:
};

#endif // !ERROR_H

