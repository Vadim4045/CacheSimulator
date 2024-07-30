#include "test_runner.h"
#include "cache.h"
#include "common.h"
#include "ddr.h"
#include "config_parser.h"
#include "loger.h"

#define MAX_REG_NAME_LENGTH 8
#define MAX_LINE_LENGTH 1024

static unsigned long trace_counter;
static unsigned long total_oper;
static unsigned long total_cost;
static unsigned long l1_hit_counter, l2_hit_counter, l3_hit_counter;
static unsigned long l1_wb_counter, l2_wb_counter, l3_wb_counter;
static unsigned int  miss_counter, l2_sw_counter, l3_sw_counter;
static unsigned long cas_counter, ras_counter;

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

RET_STATUS do_instruction(cache* cache, char instruction, unsigned int *addr, unsigned int *log)
{
	RET_STATUS ret;
	if (instruction == 'l')
	{
		ret = cache_read(cache, addr, log);
	}
	else if (instruction == 's')
	{
		ret = cache_write(cache, addr, log);
	}
	else ret = RET_WRONG;

	return ret;
}

RET_STATUS process_line(cache *cache, config *config, int settings, unsigned int *addr, char instruction)
{
	RET_STATUS ret;
	unsigned int addr_wb, trace_counter, instruction_counter, log;
	unsigned int wb_cost = config->page_size / config->bus_width;
	
	addr_wb = *addr;
	if (settings >> 4)
	{
		log |= 1 << 31; // disable date raising to L1 on L2/L3 HIT()
	}
	
	log = (settings >> 4) << 31; // disable date raising to L1 on L2/L3 HIT()
	ret = do_instruction(cache, instruction, addr, &log);

	if ((log >> 4) & 1)
	{
		log |= (1 << 15); // CAS
		if (access_to_ddr(&(cache->ddr), addr_wb))
		{
			log |= (1 << 16); // RAS
		}
	}

	if ((log >> 12) & 1)
	{
		log |= (1 << 17); // CAS
		if (access_to_ddr(&(cache->ddr), *addr))
		{
			log |= (1 << 18); // RAS
		}
	}

	total_cost += ((log >> 0) & 1) * config->cache_cfg.cache_configs[0].cost			  // L1 HIT
				  + ((log >> 1) & 1) * config->cache_cfg.cache_configs[1].cost			  // L2 HIT
				  + ((log >> 2) & 1) * config->cache_cfg.cache_configs[2].cost			  // L3 HIT
				  + wb_cost * ((log >> 15) & 1) * config->ddr_cfg.CAS					  // L3 MISS(DDR CAS)
				  + ((log >> 16) & 1) * config->ddr_cfg.RAS								  // L3 MISS(DDR RAS)
				  + wb_cost * ((log >> 5) & 1) * config->cache_cfg.cache_configs[1].cost  // L1 write-back
				  + wb_cost * ((log >> 6) & 1) * config->cache_cfg.cache_configs[2].cost  // L2 write-back
				  + wb_cost * ((log >> 7) & 1) * 100									  // L3 write-back;
				  + wb_cost * ((log >> 10) & 1) * config->cache_cfg.cache_configs[1].cost // L1<->L2 swap
				  + wb_cost * ((log >> 11) & 1) * config->cache_cfg.cache_configs[2].cost // L2<->L3 swap
				  + wb_cost * ((log >> 17) & 1) * config->ddr_cfg.CAS					  // L3 write-back after swap(DDR CAS)
				  + ((log >> 18) & 1) * config->ddr_cfg.RAS;							  // L3 write-back after swap(DDR RAS)

	l1_hit_counter += (((log >> 0) & 1) & !(((log >> 1) & 1) | ((log >> 2) & 1) | ((log >> 4) & 1)));
	l2_hit_counter += (((log >> 1) & 1) & !(((log >> 2) & 1) | ((log >> 4) & 1)));
	l3_hit_counter += (((log >> 2) & 1) & !((log >> 4) & 1));
	l1_wb_counter += ((log >> 5) & 1);
	l2_wb_counter += ((log >> 6) & 1);
	l3_wb_counter += ((log >> 7) & 1);
	miss_counter += ((log >> 4) & 1);
	l2_sw_counter += ((log >> 11) & 1);
	l3_sw_counter += ((log >> 12) & 1);
	cas_counter += (((log >> 15) & 1) + ((log >> 17) & 1));
	ras_counter += (((log >> 16) & 1) + ((log >> 18) & 1));

	if (settings & 1)
	{
		add_line(total_oper, trace_counter, *addr, log);
	}
	++total_oper;
	
	return ret;
}

int go_trace(char *trace_f, cache *cache, config *config, int settings)
{
	FILE *file;
	RET_STATUS ret;
	Register registers[32]; // Assuming 32 registers
	char line[MAX_LINE_LENGTH];
	char instruction[MAX_REG_NAME_LENGTH], reg1[MAX_REG_NAME_LENGTH], reg2[MAX_REG_NAME_LENGTH];
	unsigned long instraction_counter;
	int value1, value2;
	int num_regs = 0;
	unsigned int addr;
	unsigned int wb_cost = config->page_size / config->bus_width;
	

	file = fopen(trace_f, "r");
	if (!file)
	{
		printf("Error: opening trace file %s\n", trace_f);
		return -1;
	}

	if (settings & 1)
	{
		if (init_log_file(config, trace_f))
		{
			printf("\nError: can't make log file\n");
			exit(EXIT_FAILURE);
		}
	}

	while (fgets(line, sizeof(line), file))
	{
		++trace_counter;
		if (sscanf(line, "Info 'riscvOVPsim/cpu', 0x%lx(%*[^+]+%*x): %*x    %s %[^,], %d(%[^)])", &instraction_counter, instruction, reg1, &value1, reg2) == 5)
		{
			if (instruction[0] == 'l' || instruction[0] == 's')
			{
				int reg_idx = find_register(registers, num_regs, reg2);
				if (reg_idx != -1)
				{
					addr = registers[reg_idx].value + value1;
					ret = process_line(cache, config, settings, &addr, instruction[0]);
					++trace_counter;
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
				if (registers[reg_idx].value != value1)
				{
					printf("ERROR: register %s was 0x%x and in trace 0x%x\n", registers[reg_idx].reg, registers[reg_idx].value, value1);
				}
				registers[reg_idx].value = value2;
			}
		}
	}

	if (settings & 2)
	{
		print_cache(cache);
	}

	if(settings & 4)
	{
		while ((addr = cache_invalidate_step(cache)) > 0)
		{
			++total_oper;
			total_cost += wb_cost * config->ddr_cfg.CAS;
			if (access_to_ddr(&(cache->ddr), addr))
			{
				total_cost += wb_cost * config->ddr_cfg.RAS;
			}
			
		}
	}

	if (settings & 8)
	{
		add_to_avg_log(config, l1_hit_counter, l2_hit_counter, l3_hit_counter, l1_wb_counter
						, l2_wb_counter, l3_wb_counter, miss_counter, l2_sw_counter, l3_sw_counter
						, cas_counter, ras_counter, total_cost, total_oper, trace_counter);
	}
		
	if (settings & 1)
	{
		end_logging();
	}

	fclose(file);
	
	printf("l1_hit_rate = %f, l2_hit_rate = %f, l3_hit_rate = %f, miss_rate = %f\n", (double)l1_hit_counter / total_oper, (double)l2_hit_counter / total_oper, (double)l3_hit_counter / total_oper, (double)miss_counter / total_oper);
	printf("l1_wb_rate = %f, l2_wb_rate = %f, l3_wb_rate = %f\n", (double)l1_wb_counter / total_oper, (double)l2_wb_counter / total_oper, (double)l3_wb_counter / total_oper);
	printf("l2_sw_rate = %f, l3_sw_rate = %f\ncas_rate = %f, ras_rate = %f\n", (double)l2_sw_counter / total_oper, (double)l3_sw_counter / total_oper, (double)cas_counter / total_oper, (double)cas_counter / total_oper);
	printf("Total access: %lu (access_rate: %f), total cost: %lu, avg: %f\n", total_oper, (double)total_oper / trace_counter, total_cost, (double)total_cost / total_oper);

	return 0;
}

void run_loop(char *trace_f, config *config, int settings)
{
	cache cache;
	int i, traces_count;
	char *trace_names[10];
	char input_filename[MAX_LINE_LENGTH];
	char line[MAX_LINE_LENGTH];

	print_cfg(config);
	cache_init(&cache, config);
	init_ddr(&(cache.ddr), config);

	if (trace_f != NULL)
	{
		trace_counter = 0;
		total_oper = 0;
		total_cost = 0;
		l1_hit_counter = 0;
		l2_hit_counter = 0;
		l3_hit_counter = 0;
		l1_wb_counter = 0;
		l2_wb_counter = 0;
		l3_wb_counter = 0;
		miss_counter = 0;
		l2_sw_counter = 0;
		l3_sw_counter = 0;
		cas_counter = 0;
		ras_counter = 0;

		sprintf(input_filename, "%s", trace_f);
		printf("\nTrace\t%s:\n", input_filename);
		go_trace(input_filename, &cache, config, settings);
	}
	else
	{
		traces_count = get_all_files_in_dir("./traces", trace_names, 10);
		for (i = 0; i < traces_count; ++i)
		{
			if ((sscanf(trace_names[i], "%55[^.].%55s", line, input_filename) == 2) && !strcmp(input_filename, "trc"))
			{
				trace_counter = 0;
				total_oper = 0;
				total_cost = 0;
				l1_hit_counter = 0;
				l2_hit_counter = 0;
				l3_hit_counter = 0;
				l1_wb_counter = 0;
				l2_wb_counter = 0;
				l3_wb_counter = 0;
				miss_counter = 0;
				l2_sw_counter = 0;
				l3_sw_counter = 0;
				cas_counter = 0;
				ras_counter = 0;
				
				sprintf(input_filename, "./traces/%s", trace_names[i]);
				printf("\nTrace\t%s:\n", trace_names[i]);
				go_trace(input_filename, &cache, config, settings);
			}
		}
	}

	release_cache(&cache);
}

int runner(char *trace_f, char *config_f, char *type, unsigned int num_params, unsigned int *params_arr, int settings)
{
	int i;
	static char range_param[MAX_LINE_LENGTH];
	config config;
	
	read_config(&config, config_f);

	if(type != NULL)
	{
		for (i = 0; i < num_params; ++i)
		{
			sprintf(range_param, "%s = 0x%x", type, params_arr[i]);
			if(!parce_line(&config, range_param))
			{
				run_loop(trace_f, &config, settings);
			}
		}
	}
	else
	{
		run_loop(trace_f, &config, settings);
	}
	
	return 0;
}