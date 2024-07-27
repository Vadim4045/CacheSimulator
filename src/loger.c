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
	
	sprintf(buff_1, "./logs/%s_%u_%u_%u_%u_%u_", buff_2
			, config->bus_width
			, config->page_size
			, config->cache_cfg.cache_levels
			, config->ddr_cfg.channels
			, config->ddr_cfg.IP
			);
			
	for(i = 0; i < config->cache_cfg.cache_levels; ++i)
	{
		sprintf(buff_1, "%s_%x_%u", buff_1
				, config->cache_cfg.cache_configs[i].size
				, config->cache_cfg.cache_configs[i].sets
				);
	}
	
	for (i = 0; i < config->ddr_cfg.channels; ++i)
	{
		sprintf(buff_1, "%s_%x_%u_%u_%u_%u", buff_1
				, config->ddr_cfg.channels
				, config->ddr_cfg.channels_config.num_of_dimms
				, config->ddr_cfg.channels_config.dimms.num_of_ranks
				, config->ddr_cfg.channels_config.dimms.rank.num_of_banks
				);
	}

	sprintf(buff_1, "%s.csv", buff_1);

	file = fopen(buff_1, "w");
	if (!file)
	{
		printf(buff_1);
		return True;
	}

	fprintf(file, "Trace_No;Programm_No;Addr;L1_HIT;L2_HIT;L3_HIT;L3_MISS;L1_WB;L2_WB;L3_WB;L1_SWAP;L2_SWAP;L3_SWAP;DDR_CAS;DDR_RAS\n");
	return False;
}

BOOL add_line(unsigned int tr_num, unsigned int ip, unsigned int addr, unsigned int log){
	fprintf(file, "%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;0;0\n", tr_num, ip, addr
			, ((log >> 0) & 1), ((log >> 1) & 1), ((log >> 2) & 1)
			, ((log >> 4) & 1), ((log >> 5) & 1), ((log >> 6) & 1)
			, ((log >> 7) & 1), ((log >> 10) & 1), ((log >> 11) & 1), ((log >> 12) & 1));
			
	return True;
}

BOOL add_to_avg_log(config *config, unsigned int trc_avg, unsigned int prg_avg)
{
	unsigned int i;

	sprintf(buff_1, "./logs/AVG_scores.csv");

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

		fprintf(file, "Bus;Page_size;Levels;DDR_channels;IP;L1_size;L1_sets;L2_size;L2_sets;L3_size;L3_sets;DDR_CH1_size;DDR_CH1_dims;DDR_CH1_banks;Trace_AVG;Prog_AVG\n");
	}

	fprintf(file, "%u;%u;%u;%u;%u;"
			, config->bus_width
			, config->page_size
			, config->cache_cfg.cache_levels
			, config->ddr_cfg.channels
			, config->ddr_cfg.IP
			);

	for (i = 0; i < 3; ++i)
	{
		fprintf(file, "%u;%u;"
			,config->cache_cfg.cache_configs[i].size
			, config->cache_cfg.cache_configs[i].sets
			);
	}

	fprintf(file, "%u;%u;%u;%u;", config->ddr_cfg.size, config->ddr_cfg.channels, config->ddr_cfg.channels_config.num_of_dimms, config->ddr_cfg.channels_config.dimms.num_of_ranks, config->ddr_cfg.channels_config.dimms.rank.num_of_banks);

	fprintf(file,"%u;%u\n", trc_avg, prg_avg);
	fclose(file);
	return False;
}

BOOL end_logging()
{
	return fclose(file);
}