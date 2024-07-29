#include "loger.h"

#include "common.h"

static char buff_1[MAX_LINE_LENGTH];
static char buff_2[MAX_LINE_LENGTH];
FILE *file;

BOOL init_log_file(config *config, char* trace)
{
	unsigned int i;

	replace_simbol(trace, '_', ' ');
	sscanf(trace, "./traces/%s", buff_2);
	
	sprintf(buff_1, "./logs/%s_Page_%u_Levels_%u", buff_2
			, config->page_size
			, config->cache_cfg.cache_levels
			);
			
	for(i = 0; i < config->cache_cfg.cache_levels; ++i)
	{
		sprintf(buff_1, "%s_p_size_%x_sets_%u", buff_1
				, config->cache_cfg.cache_configs[i].size / _1K
				, config->cache_cfg.cache_configs[i].sets
				);
	}
	

	sprintf(buff_1, "%s_ddr_banks_%u_row_size%x_ip_%u", buff_1
			, config->ddr_cfg.num_of_banks
			, config->ddr_cfg.row_size / _1K
			, config->ddr_cfg.IP
			);


	sprintf(buff_1, "%s.csv", buff_1);

	file = fopen(buff_1, "w");
	if (!file)
	{
		printf(buff_1);
		return True;
	}

	fprintf(file, "Trace_No;Addr;L1_HIT;L2_HIT;L3_HIT;L1_WB;L2_WB;L3_WB;L2_SWAP;L3_SWAP;DDR_CAS;DDR_RAS\n");
	return False;
}

BOOL add_line(unsigned int tr_num, unsigned int ip, unsigned int addr, unsigned int log){
	fprintf(file, "%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u\n", tr_num, addr
			, ((log >> 0) & 1), ((log >> 1) & 1), ((log >> 2) & 1) // levels HIT
			, ((log >> 5) & 1), ((log >> 6) & 1), ((log >> 7) & 1) // levels write-back
			, ((log >> 11) & 1), ((log >> 12) & 1) // swap costs
			, (((log >> 15) & 1) + ((log >> 17) & 1)) // DDR CAS
			, (((log >> 16) & 1) + ((log >> 18) & 1)) // DDR RAS
	);

	return True;
}

BOOL add_to_avg_log(config *config, unsigned int trc_avg, unsigned int prg_avg)
{
	unsigned int i;

	sprintf(buff_1, "./logs/Cache_simulator_statistic.csv");

	if (access(buff_1, F_OK) == 0)
	{
		file = fopen(buff_1, "a");
		if (!file)
		{
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

		fprintf(file, "Page_size;Levels;L1_size;L1_sets;L2_size;L2_sets;L3_size;L3_sets;DDR_banks;DDR_row_size;IP;Trace_AVG;Prog_AVG\n");
	}

	fprintf(file, "%u;%u;"
			, config->page_size
			, config->cache_cfg.cache_levels
			);

	for (i = 0; i < 3; ++i)
	{
		fprintf(file, "%u;%u;"
			, config->cache_cfg.cache_configs[i].size / _1K
			, config->cache_cfg.cache_configs[i].sets
			);
	}

	fprintf(file, "%u;%u;%u;%u;%u\n"
		, config->ddr_cfg.num_of_banks
		, config->ddr_cfg.row_size / _1K
		, config->ddr_cfg.IP
		, trc_avg, prg_avg);

	fclose(file);
	return False;
}

BOOL end_logging()
{
	return fclose(file);
}