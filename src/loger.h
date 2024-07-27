#ifndef LOGER_H
#define LOGER_H

#include "defines.h"

BOOL init_log_file(config *config);
BOOL add_line(unsigned int log);
BOOL end_logging();
#endif // LOGER_H