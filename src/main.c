#include "test_runner.h"

#define MAX_PARAMS 10
#define RANGES 9

const char *range_types[RANGES] = {
	"page_size",
	"cache_levels",
	"level1_size",
	"level2_size",
	"level3_size",
	"write_policy",
	"replaсement",
	"associativity",
	"interleaving"};

void print_usage()
{
	char line[MAX_LINE_LENGTH];
	FILE *file = fopen("./README.txt", "r");

	if (!file)
	{
		printf("Error: opening Readme file\n");
		exit(EXIT_FAILURE);
	}

	while (fgets(line, sizeof(line), file))
	{
		printf(line);
	}
	
	fclose(file);
}	
	
int main(int argc, char **argv)
{
	uint numbers[MAX_PARAMS];
	int opt;
	char *trace_file_name = NULL;
	char *config_file_name = NULL;
	char *range_type = NULL;
	int show_help = 0, range_num = -1;

	while ((opt = getopt(argc, argv, "hf:")) != -1)
	{
		switch (opt)
		{
		case 'h':
			show_help = 1;
			break;
		case 't':
			trace_file_name = optarg;
			break;
		case 'c':
			config_file_name = optarg;
			break;
		case 'r':
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
			opt = atoi(argv[optind + i]);
			if(opt && is_power_of_2(opt))
			{
				numbers[i] = opt;
			}
			else
			{
				fprintf(stderr, "Error: All values must be power of two\n");
				exit(EXIT_FAILURE);
			}
			
		}
	}

	if (runner(trace_file_name, config_file_name, range_num, num_count, numbers)){
		fprintf(stderr, "Error: Simulation Failed\n");
		exit(EXIT_FAILURE);
	}
		
	exit(EXIT_SUCCESS);
}