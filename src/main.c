
#include "test_runner.h"
#include "common.h"

#define MAX_PARAMS 10
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
		printf("%s",line);
	}
	
	fclose(file);
}

int main(int argc, char **argv)
{
	unsigned int numbers[MAX_PARAMS];
	int opt, num_count;
	char *trace_file_name = NULL;
	char *config_file_name = NULL;
	char *range_type = NULL;
	int show_help = 0, range_num = -1, settings = 0;

	while ((opt = getopt(argc, argv, "hdlvmpit:c:r:")) != -1)
	{
		switch (opt)
		{
		case 'h':
			show_help = 1;
			break;
		case 'v':
			settings |= (1 << 0);
			break;
		case 'm':
			settings |= (1 << 1);
			break;
		case 'i':
			settings |= (1 << 2);
			break;
		case 'l':
			settings |= (1 << 3);
			break;
		case 'd':
			settings |= (1 << 4);
			break;
		case 'p':
			settings |= (1 << 5);
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

	if(show_help)
	{
		print_usage();
		exit(EXIT_SUCCESS);
	}

	
	if (range_type != NULL)
	{
		num_count = argc - optind;
		if (num_count <= 0 || num_count >= MAX_PARAMS)
		{
			fprintf(stderr, "Error: Range type or values wrong\n");
			print_usage();
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < num_count; i++)
		{
			int val = atoi(argv[optind + i]);
			numbers[i] = val;
		}
	}

	if (runner(trace_file_name, config_file_name, range_type, num_count, numbers, settings))
	{
		fprintf(stderr, "Error: Simulation Failed\n");
		exit(EXIT_FAILURE);
	}

	printf("Simulation SUCCESS\n");
	exit(EXIT_SUCCESS);
}