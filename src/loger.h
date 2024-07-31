#ifndef LOGER_H
#define LOGER_H

#include "defines.h"

BOOL init_log_file(config *config, char *trace);
BOOL add_line(unsigned int tr_num, unsigned int ip, unsigned int addr, unsigned int log);
BOOL add_to_avg_log(config *config, unsigned int l1_hit_counter, unsigned int l2_hit_counter
					, unsigned int l3_hit_counter, unsigned int l1_wb_counter, unsigned int l2_wb_counter
					, unsigned int l3_wb_counter, unsigned int miss_counter, unsigned int l2_sw_counter
					, unsigned int l3_sw_counter, unsigned int cas_counter, unsigned int ras_counter
					, unsigned int total_cost, unsigned int total_oper, unsigned int trace_counter, unsigned int RAS);
BOOL end_logging();
#endif // LOGER_H