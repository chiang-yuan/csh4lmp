#include "error.h"

Error::Error()
{
	mode = basic;
	level = stop;
}

Error::~Error()
{
}

void Error::setMode(int mode_)
{
	if (mode_ == silent) mode = silent;
	else if (mode_ == basic) mode = basic;
	else if (mode_ == verbose) mode = verbose;
	else {
		mode = verbose;
		message("Invalid error mode setting", -1);
	}
}

void Error::setLevel(int level_)
{
	if (level_ == ignore) level = ignore;
	else if (level_ == stop) level = stop;
	else {
		level = stop;
		message("Invalid error level setting", -1);
	}
}

int Error::warning(const char * warning_, int warncode_)
{
	if (mode != silent) printf("WARNNING: %s\n", warning_);

	return warncode_;
}

int Error::warning(const char * warning_, int warncode_, const char* str)
{
	char* buffer = new char[MAX_STRING];
	sprintf(buffer, warning_, str);
	if (mode != silent) printf("WARNNING: %s\n", buffer);

	return warncode_;
}

int Error::message(const char * message_, int exitcode_)
{
	if (mode != silent) printf("ERROR: %s\n", message_);

	if (level == stop) {
		exit(exitcode_);
	}
	return exitcode_;
}

int Error::message(const char * message_, int exitcode_, const char* str)
{
	char* buffer = new char[MAX_STRING];
	sprintf(buffer, message_, str);
	if (mode != silent) printf("ERROR: %s\n", buffer);
	
	if (level == stop) {
		exit(exitcode_);
	}
	return exitcode_;
}
