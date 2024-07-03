#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_REG_NAME_LENGTH 8

typedef struct
{
	char reg[MAX_REG_NAME_LENGTH];
	int value;
} Register;

int find_register(Register *registers, int num_regs, const char *reg_name)
{
	for (int i = 0; i < num_regs; ++i)
	{
		if (strcmp(registers[i].reg, reg_name) == 0)
		{
			return i;
		}
	}
	return -1;
}

void process_trace_file(char *input_filename, char *output_filename)
{
	FILE *infile = fopen(input_filename, "r");
	FILE *outfile = fopen(output_filename, "w");

	if (!infile)
	{
		printf("Error: opening input file: %s", input_filename);
		exit(EXIT_FAILURE);
	}
	if (!outfile)
	{
		printf("Error: opening output file: %s", output_filename);
		exit(EXIT_FAILURE);
	}

	char line[MAX_LINE_LENGTH];
	Register registers[32]; // Assuming 32 registers
	int num_regs = 0;
	unsigned long instraction_counter, trace_counter;
	char instruction[MAX_REG_NAME_LENGTH], reg1[MAX_REG_NAME_LENGTH], reg2[MAX_REG_NAME_LENGTH];
	int value1, value2;
	trace_counter = 0;
	while (fgets(line, sizeof(line), infile))
	{
		++trace_counter;
		if (sscanf(line, "Info 'riscvOVPsim/cpu', 0x%lx(%*[^+]+%*x): %*x    %s %[^,], %d(%[^)])", &instraction_counter, instruction, reg1, &value1, reg2) == 5)
		{
			if (instruction[0] == 'l' || instruction[0] == 's'){
				int reg_idx = find_register(registers, num_regs, reg2);
				if (reg_idx != -1)
				{
					unsigned int full_address = registers[reg_idx].value + value1;
					fprintf(outfile, "0x%lx 0x%lx %s 0x%x\n", trace_counter, instraction_counter, instruction, full_address);
				}
			}	
		}
		else if (sscanf(line, "Info %s %x -> %x", reg1, &value1, &value2) == 3)
		{
			--trace_counter;
			int reg_idx = find_register(registers, num_regs, reg1);
			if (reg_idx == -1)
			{
				strcpy(registers[num_regs].reg, reg1);
				registers[num_regs].value = value2;
				num_regs++;
			}
			else
			{
				if (registers[reg_idx].value != value1){
					printf("ERROR: register %s was 0x%x and in trace 0x%x\n", registers[reg_idx].reg, registers[reg_idx].value, value1);
				}
				registers[reg_idx].value = value2;
			}
		}
	}

	fclose(infile);
	fclose(outfile);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Wrong parameter\n");
		return 1;
	}

	char input_filename[MAX_LINE_LENGTH] = "./raw_traces/";
	char output_filename[MAX_LINE_LENGTH] = "new_";

	if (strlen(input_filename) + strlen(argv[1]) >= MAX_LINE_LENGTH)
	{
		fprintf(stderr, "Input filename too long\n");
		return 1;
	}

	if (strlen(output_filename) + strlen(argv[1]) >= MAX_LINE_LENGTH)
	{
		fprintf(stderr, "Output filename too long\n");
		return 1;
	}

	strcat(input_filename, argv[1]);
	strcat(output_filename, argv[1]);

	clock_t start_time = clock();

	process_trace_file(input_filename, output_filename);

	clock_t end_time = clock();
	double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

	printf("Processing complete. Output written to %s\n", output_filename);
	printf("Execution time: %f seconds\n", execution_time);

	return 0;
}
