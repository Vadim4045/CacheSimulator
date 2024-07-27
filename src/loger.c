#include "loger.h"

#include "common.h"

static char buff_1[MAX_LINE_LENGTH];
static char buff_2[MAX_LINE_LENGTH];
FILE *file;

BOOL init_log_file(config *config)
{
	unsigned int i;

	sprintf(buff_1, "./logs/_%u_%u_%u_%u_%u_"
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
				, config->cache_cfg.cache_configs[i].sets);
	}
	
	for (i = 0; i < config->ddr_cfg.channels; ++i)
	{
		sprintf(buff_1, "%s_%x_%u_%x_%u_%u", buff_1
				, config->ddr_cfg.channels_config[i].size
				, config->ddr_cfg.channels_config[i].dimms
				, config->ddr_cfg.channels_config[i].banks);
	}

	sprintf(buff_1, "%s.csv", buff_1);

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

		fprintf(file, "Trace_No;Programm_No;Addr;L1_HIT;L2_HIT;L3_HIT;L3_MISS;L1_WB;L2_WB;L3_WB;L1_SWAP;L2_SWAP;L3_SWAP;DDR_CAS;DDR_RAS\n");
	}
	
return False;
}

BOOL add_line(unsigned int tr_num, unsigned int ip, unsigned int addr, unsigned int log){
	fprintf(file, "0x%x;0x%x;0x%x;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;0;0\n", tr_num, ip, addr
			, ((log >> 0) & 1), ((log >> 1) & 1), ((log >> 2) & 1)
			, ((log >> 4) & 1), ((log >> 5) & 1), ((log >> 6) & 1)
			, ((log >> 7) & 1), ((log >> 10) & 1), ((log >> 11) & 1), ((log >> 12) & 1));
			
	return True;
}

BOOL end_logging()
{
	return fclose(file);
}