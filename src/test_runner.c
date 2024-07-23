#include "test_runner.h"
#include "cache_simulator.h"
//#include "ddr_simulator.h"
#include "config_parser.h"
//#include "loger.h"

int run_test(char *trace_f, cache_config *config)
{
	char log_filename[MAX_LINE_LENGTH];
	char verbose_log_filename[MAX_LINE_LENGTH];
	char trace_filename[MAX_LINE_LENGTH];
	
	sprintf(verbose_log_filename, "./logs/%s_page%d_levels%d.%d.%d_lvl_sets%d.%d.%d.log"
		 , trace_f
		 , config->page_size
		 , config->cache_configurations[0].size
		 , config->cache_configurations[1].size
		 , config->cache_configurations[2].size
		 , config->cache_configurations[0].sets
		 , config->cache_configurations[1].sets
		 , config->cache_configurations[2].sets);



	return 0;
}

int runner(char *trace_f, char *config_f, int type, uint num_params, uint *params_arr)
{
	int i;
	cache_config config;
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
		return run_test(trace_f, &config);
	}

	for (int i = 0; i < num_params; ++i)
	{
		set_param(&config, type, params_arr[i]);

		if (run_test(trace_f, &config))
		{
			fprintf(stderr, "Error: Test filed\n");
			return 1;
		}
	}

	return 0;
}