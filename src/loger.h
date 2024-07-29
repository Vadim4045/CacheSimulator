#ifndef LOGER_H
#define LOGER_H

#include "defines.h"

BOOL init_log_file(config *config, char *trace);
BOOL add_line(unsigned int tr_num, unsigned int ip, unsigned int addr, unsigned int log);
BOOL add_to_avg_log(config *config, float trc_avg, float prg_avg);
BOOL end_logging();
#endif // LOGER_H