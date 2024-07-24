#include "test_runner.h"

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
	int opt;
	char *trace_file_name = NULL;
	char *config_file_name = NULL;
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

	if (runner(trace_file_name, config_file_name)){
		fprintf(stderr, "Error: Simulation Failed\n");
		exit(EXIT_FAILURE);
	}
		
	exit(EXIT_SUCCESS);
}