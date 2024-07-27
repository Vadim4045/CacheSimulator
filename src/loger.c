#include "loger.h"

#include "common.h"

static char log_filename[MAX_LINE_LENGTH];
FILE *file;

BOOL init_log_file(config *config)
{

	file = fopen(log_filename, "a");
}

BOOL add_line(unsigned int log)
{
	
}

BOOL end_logging()
{
	return fclose(file);
}