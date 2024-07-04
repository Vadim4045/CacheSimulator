#include "test_runner.h"
#include "cache_simulator.h"
#include "ddr_simulator.h"
#include "config_parser.h"
#include "loger.h"

int run_test(char *trace, cache_config *config)
{
	char output_filename[MAX_LINE_LENGTH] = "../logs/";
}

int runner(char *trace, int type, uint num_params, uint *params_arr)
{
	int i;
	cache_config config;
	char input_filename[MAX_LINE_LENGTH] = "../traces/";
	strcat(input_filename, trace);
	if (access(input_filename, F_OK) == -1)
	{
		fprintf(stderr, "Error: File '%s' does not exist\n", input_filename);
		return 1;
	}

	if (type == -1)
	{
		return run_test(trace, &config);
	}

	for (int i = 0; i < num_params; ++i)
	{
		set_param(&config, type, params_arr[i]);

		if (run_test(trace, &config))
		{
			fprintf(stderr, "Error: Test filed\n");
			return 1;
		}
	}

	return 0;
}