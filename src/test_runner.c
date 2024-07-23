#include "test_runner.h"
#include "cache_simulator.h"
//#include "ddr_simulator.h"
#include "config_parser.h"
//#include "loger.h"

int do_instruction(char* instruction_string, uint addr, uint* cost)
{
	DEBUG("Readed line: instruction_string = %s, addr 0x%x\n", instruction_string, addr);
}

int run_test(char *trace_f, config *config)
{
	DEBUG("TestRunner_%s\n", __FUNCTION__);
	uint err_counter = 0;
	uint cost, addr, trace_counter, instruction_counter;
	char instruction_string[8];
	
	char log_filename[MAX_LINE_LENGTH];
	char verbose_log_filename[MAX_LINE_LENGTH];
	char line[MAX_LINE_LENGTH];

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
			do_instruction(instruction_string, addr, &cost);
		}
		else{
			printf("Error: reading line %s\n", line);
			++err_counter;
			continue;
		}
	}

	fclose(file);

	return 0;
}

int runner(char *trace_f, char *config_f, int type, uint num_params, uint *params_arr)
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

	init_simulator(&config, config_f);

	if (type == -1)
	{
		return run_test(input_filename, &config);
	}

	for (int i = 0; i < num_params; ++i)
	{
		set_param(&config, type, params_arr[i]);

		if (run_test(input_filename, &config))
		{
			fprintf(stderr, "Error: Test filed\n");
			return 1;
		}
	}

	return 0;
}