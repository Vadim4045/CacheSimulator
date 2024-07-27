#ifndef LOGER_H
#define LOGER_H

#include "defines.h"

BOOL init_log_file(config *config, char *trace);
BOOL add_line(unsigned int tr_num, unsigned int ip, unsigned int addr, unsigned int log);
BOOL add_to_avg_log(config *config, unsigned int trc_avg, unsigned int prg_avg);
BOOL end_logging();
#endif // LOGER_H