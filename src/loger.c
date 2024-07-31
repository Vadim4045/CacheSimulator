#include "loger.h"
#include "common.h"

#include <stdio.h>
#include <errno.h>

#if defined(_WIN32)
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#elif defined(__linux__) || defined(__APPLE__)
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(path) mkdir(path, 0700)
#else
#error "System not support"
#endif

static char buff_1[MAX_LINE_LENGTH];
static char buff_2[MAX_LINE_LENGTH];
FILE *file;

BOOL init_log_file(config *config, char* trace)
{
	unsigned int i;

	replace_simbol(trace, '_', ' ');
	sscanf(trace, "./traces/%s", buff_2);

	sprintf(buff_1, "./logs/%s_page_%u_levels_%u"
			, buff_2
			, config->page_size
			, config->cache_cfg.cache_levels
			);

	for(i = 0; i < config->cache_cfg.cache_levels; ++i)
	{
		sprintf(buff_1, "%s_L%u_size_%x_sets_%u", buff_1, i + 1
				, config->cache_cfg.cache_configs[i].size / _1K
				, config->cache_cfg.cache_configs[i].sets
				);
	}

	sprintf(buff_1, "%s_ddr_banks_%u_row_size%x_ch_pos_%u_ip_%u", buff_1
			, config->ddr_cfg.num_of_banks
			, config->ddr_cfg.row_size / _1K
			, config->ddr_cfg.channel_pos
			, config->ddr_cfg.IP
			);

	sprintf(buff_1, "%s.csv", buff_1);

	if (MKDIR("./logs") && errno != EEXIST)
	{
		perror("Can't make folder './logs'\n");
		return True;
	}
	
	file = fopen(buff_1, "w");
	if (!file)
	{
		printf("Can't make file '%s'\n", buff_1);
		return True;
	}
	
	fprintf(file, "Trace_No;Addr;L1_HIT;L2_HIT;L3_HIT;MISS;L1_WB;L2_WB;L3_WB;L2_SWAP;L3_SWAP;DDR_CAS;DDR_RAS\n");
	return False;
}

BOOL add_line(unsigned int tr_num, unsigned int ip, unsigned int addr, unsigned int log){
	fprintf(file, "%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u\n", tr_num, addr
			, (((log >> 0) & 1) & !(((log >> 1) & 1) | ((log >> 2) & 1) | ((log >> 4) & 1))) // L1 HIT
			,(((log >> 1) & 1) & !(((log >> 2) & 1) | ((log >> 4) & 1))) // L2 HIT
			,(((log >> 2) & 1) & !((log >> 4) & 1)) // L3 HIT
			,((log >> 4) & 1) // MISS
			,((log >> 5) & 1), ((log >> 6) & 1), ((log >> 7) & 1) // levels write-back
			,((log >> 11) & 1), ((log >> 12) & 1) // swap costs
			,(((log >> 15) & 1) + ((log >> 17) & 1)) // DDR CAS
			,(((log >> 16) & 1) + ((log >> 18) & 1)) // DDR RAS
	);

	return True;
}

BOOL add_to_avg_log(config *config, unsigned int l1_hit_counter, unsigned int l2_hit_counter, unsigned int l3_hit_counter
					, unsigned int l1_wb_counter, unsigned int l2_wb_counter, unsigned int l3_wb_counter, unsigned int miss_counter
					, unsigned int l2_sw_counter, unsigned int l3_sw_counter, unsigned int cas_counter, unsigned int ras_counter
					, unsigned int total_cost, unsigned int total_oper, unsigned int trace_counter, unsigned int RAS)
{
	unsigned int i;

	sprintf(buff_1, "./logs/Cache_simulator_statistic.csv");

	if (access(buff_1, F_OK) == 0)
	{
		if (MKDIR("./logs") && errno != EEXIST)
		{
			perror("Can't make folder './logs'\n");
			return True;
		}
		
		file = fopen(buff_1, "a");
		if (!file)
		{
			printf("Can't make file '%s'\n", buff_1);
			return True;
		}
	}
	else
	{
		file = fopen(buff_1, "w");
		if (!file)
		{
			return True;
		}

		fprintf(file, "Page_size;Levels;L1_size;L1_sets;L2_size;L2_sets;L3_size;L3_sets;DDR_ch;DDR_banks;DDR_row_size;Ch_pos;IP;DDR RAS;");
		fprintf(file, "L1_HIT_rate;L2_HIT_rate;L3_HIT_rate;MISS_rate;");
		fprintf(file, "L1_WB_rate;L2_WB_rate;L3_WB_rate;");
		fprintf(file, "L2_swap_rate;L3_swap_rate;DDR_CAS_rate;DDR_RAS_rate;");
		fprintf(file, "AVG cache access cost\n");
	}

	fprintf(file, "%u;%u;", config->page_size, config->cache_cfg.cache_levels);

	for (i = 0; i < 3; ++i)
	{
		fprintf(file, "%u;%u;"
			, config->cache_cfg.cache_configs[i].size / _1K
			, config->cache_cfg.cache_configs[i].sets
			);
	}

	fprintf(file, "%u;%u;%u;%u;%u;%u;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f\n"
			, config->ddr_cfg.num_of_channels
			, config->ddr_cfg.num_of_banks
			, config->ddr_cfg.row_size / _1K
			, config->ddr_cfg.channel_pos
			, config->ddr_cfg.IP
			, RAS
			, (double)l1_hit_counter / total_oper
			, (double)l2_hit_counter / total_oper
			, (double)l3_hit_counter / total_oper
			, (double)miss_counter / total_oper
			, (double)l1_wb_counter / total_oper
			, (double)l2_wb_counter / total_oper
			, (double)l3_wb_counter / total_oper
			, (double)l2_sw_counter / total_oper
			, (double)l3_sw_counter / total_oper
			, (double)cas_counter / total_oper
			, (double)ras_counter / total_oper
			, (double)total_cost / total_oper
			);

	fclose(file);
	return False;
}

BOOL end_logging()
{
	return fclose(file);
}