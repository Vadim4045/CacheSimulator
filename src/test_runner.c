#include "test_runner.h"
#include "cache/cache.h"
#include "common.h"
//#include "ddr_simulator.h"
#include "config_parser/config_parser.h"
//#include "loger.h"

RET_STATUS do_instruction(cache* cache, char *instruction_string, unsigned int addr, unsigned int *log)
{
	RET_STATUS ret;
	DEBUG("\n%s Readed line: instruction_string = %s, addr 0x%x\n", __FUNCTION__, instruction_string, addr);
	if (instruction_string[0] == 'l')
	{
		ret = cache_read(cache, &addr, log);
	}
	else if (instruction_string[0] == 's')
	{
		ret = cache_write(cache, &addr, log);
	}
	else ret = RET_WRONG;
	DEBUG("-------%s---------- got result: %s (addr = 0x%x)\n", __FUNCTION__, ret2str(ret), addr);
	return ret;
}

int run_test(char *trace_f, config *config)
{
	DEBUG("TestRunner_%s\n", __FUNCTION__);
	cache cache;
	unsigned int err_counter = 0;
	unsigned int log, addr, trace_counter, instruction_counter, ret, total_cost = 0, total_oper = 0;
	char instruction_string[8];
	char log_filename[MAX_LINE_LENGTH];
	char verbose_log_filename[MAX_LINE_LENGTH];
	char line[MAX_LINE_LENGTH];
	unsigned int wb_cost = config->page_size / config->bus_width;

	sprintf(verbose_log_filename, "./logs/%s_page%d_levels%d.%d.%d_lvl_sets%d.%d.%d_ddr%d.%d.log"
			, trace_f, config->page_size
			, config->cache_cfg.cache_configs[0].size
			, config->cache_cfg.cache_configs[1].size
			, config->cache_cfg.cache_configs[2].size
			, config->cache_cfg.cache_configs[0].sets
			, config->cache_cfg.cache_configs[1].sets
			, config->cache_cfg.cache_configs[2].sets
			, config->ddr_cfg.channels
			, config->ddr_cfg.channels_config[0].dimms
			, config->ddr_cfg.channels_config[0].banks);

	cache_init(&cache, config);

	FILE *file = fopen(trace_f, "r");

	if (!file)
	{
		printf("Error: opening trace file %s\n", trace_f);
		exit(EXIT_FAILURE);
	}

	while (fgets(line, sizeof(line), file))
	{
		if((sscanf(line, "0x%x 0x%x %s 0x%x", &trace_counter, &instruction_counter, instruction_string, &addr) == 4))
		{
			log = 0;
			ret = do_instruction(&cache, instruction_string, addr, &log);
			total_cost += ((log >> 0) & 1) * config->cache_cfg.cache_configs[0].cost 
						+ ((log >> 1) & 1) * config->cache_cfg.cache_configs[1].cost 
						+ ((log >> 2) & 1) * config->cache_cfg.cache_configs[2].cost 
						+ wb_cost * ((log >> 5) & 1) * config->cache_cfg.cache_configs[0].cost 
						+ wb_cost * ((log >> 6) & 1) * config->cache_cfg.cache_configs[1].cost 
						+ wb_cost * ((log >> 7) & 1) * config->cache_cfg.cache_configs[2].cost;
			printf("Instruction '%s'  with addr 0x%x returns %s, total cost 0x%x\n", instruction_string, addr, ret2str(ret), log);
			++total_oper;
			//print_cache(&cache);
			//if(trace_counter >= 400) break;
		}
		else{
			printf("Error: reading line %s\n", line);
			++err_counter;
			continue;
		}
	}
	print_cache(&cache);
	
	fclose(file);
	printf("--------------------------Total cost = %d, per instruction %f\n", total_cost, (float)total_cost / total_oper);
	return 0;
}

int runner(char *trace_f, char *config_f)
{
	DEBUG("TestRunner_%s\n", __FUNCTION__);
	int i;
	config config;
	char input_filename[MAX_LINE_LENGTH];
	char config_filename[MAX_LINE_LENGTH];
	
	if(trace_f != NULL)
	{
		sprintf(input_filename, "./traces/%s.trc", trace_f);
	}
	else
	{
		sprintf(input_filename, "./traces/linpack_val.trc");
	}
	
	if (access(input_filename, F_OK) == -1)
	{
		fprintf(stderr, "Error: File '%s' does not exist\n", input_filename);
		return 1;
	}

	read_config(&config, config_f);
	//print_cfg(&config);

	return run_test(input_filename, &config);
}