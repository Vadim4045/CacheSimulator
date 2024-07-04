
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "defines.h"
#include "cache.h"
#include "config_parser.h"

#define MAX_PARAMS 10
#define RANGES 9

static const char* range_types[] = {
	"page_size",
	"cache_levels",
	"level1_size",
	"level2_size",
	"level3_size",
	"write_policy",
	"replaсement",
	"associativity",
	"interleaving"
};

int run_test(cache_config config)
{
	char output_filename[MAX_LINE_LENGTH] = "../logs/";
}

int runner(char* trace, int type, uint num_params, uint* params_arr)
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

	if(type == -1)
	{
		return run_test(config);
	}
		
	for (int i = 0; i < num_params; ++i)
	{
		set_param(config, type, params_arr[i]);

		if (run_test(config))
		{
			fprintf(stderr, "Error: Test filed\n");
			return 1;
		}
	}
	
	return 0;
}

int main(int argc, char** argv){
	uint numbers[MAX_PARAMS];
	int opt;
	char *file_name = NULL;
	char *range_type = NULL;
	int show_help = 0, range_num = -1;

	while ((opt = getopt(argc, argv, "hf:")) != -1)
	{
		switch (opt)
		{
		case 'h':
			show_help = 1;
			break;
		case 'f':
			file_name = optarg;
			break;
		case 't':
			range_type = optarg;
			break;
		default:
			print_usage();
			exit(EXIT_FAILURE);
		}
	}

	if (show_help)
	{
		print_usage();
		exit(EXIT_SUCCESS);
	}

	if (file_name == NULL)
	{
		fprintf(stderr, "Error: File name is required\n");
		print_usage();
		exit(EXIT_FAILURE);
	}

	int num_count = argc - optind;
	if (range_type != NULL){
		for (range_num = 0; range_num < RANGES; ++range_num)
		{
			if (strcmp(range_type, range_types[range_num]))
				break;
		}

		if (num_count <= 0 || range_num >= RANGES)
		{
			fprintf(stderr, "Error: Range type or values wrong\n");
			print_usage();
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < num_count; i++)
		{
			numbers[i] = atoi(argv[optind + i]);
		}
	}

	if (runner(file_name, range_num, num_count, numbers)){
		fprintf(stderr, "Error: Simulation Failed\n");
		exit(EXIT_FAILURE);
	}
		
	exit(EXIT_SUCCESS);
}