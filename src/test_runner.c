#include "test_runner.h"
#include "cache.h"
#include "common.h"
#include "ddr.h"
#include "config_parser.h"
#include "loger.h"

static char input_filename[MAX_LINE_LENGTH];
static char range_param[MAX_LINE_LENGTH];
static char line[MAX_LINE_LENGTH];

RET_STATUS do_instruction(cache* cache, char *instruction_string, unsigned int *addr, unsigned int *log)
{
	RET_STATUS ret;
	if (instruction_string[0] == 'l')
	{
		ret = cache_read(cache, addr, log);
	}
	else if (instruction_string[0] == 's')
	{
		ret = cache_write(cache, addr, log);
	}
	else ret = RET_WRONG;

	return ret;
}

int run_test(char *trace_f, cache *cache, ddr *ddr, config *config, int settings)
{
	unsigned int log, addr, addr_wb, trace_counter, instruction_counter, ret, total_cost = 0, total_oper = 0;
	char instruction_string[8];
	unsigned int wb_cost = config->page_size / config->bus_width;
	FILE * file;

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
		
		if((sscanf(line, "0x%x 0x%x %s 0x%x", &trace_counter, &instruction_counter, instruction_string, &addr) == 4))
		{
			addr_wb = addr;
			log = 0;
			ret = do_instruction(cache, instruction_string, &addr, &log);
	
			if ((log >> 4) & 1)
			{
				log |= (1 << 15); // CAS
				if(access_to_ddr(ddr, addr_wb))
				{
					log |= (1 << 16); // RAS
				}
			}
	
			if ((log >> 12) & 1)
			{
				log |= (1 << 17); // CAS
				if(access_to_ddr(ddr, addr))
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

			if (settings & 1)
			{
				add_line(total_oper, trace_counter, addr, log);
			}
			++ total_oper;
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
			total_cost += wb_cost * 100; // cost of one page invalidate
		}
	}

	if (settings & 8)
	{
		add_to_avg_log(config, total_cost / instruction_counter, total_cost / total_oper);
	}
		
	if (settings & 1)
	{
		end_logging();
	}

	fclose(file);
	printf("--------Total lowd/store instructions: %d,total cost: %d, avg: %f\n", total_oper, total_cost, (float)total_cost / total_oper);
	return 0;
}

void run_loop(char *trace_f, config *config, int settings)
{
	cache cache;
	ddr ddr;
	int i, traces_count;
	char *trace_names[10];

	print_cfg(config);
	cache_init(&cache, config);
	init_ddr(&ddr, config);

	if (trace_f != NULL)
	{
		sprintf(input_filename, "%s", trace_f);
		printf("Trace\t%s:\n", input_filename);
		run_test(input_filename, &cache, &ddr, config, settings);
	}
	else
	{
		traces_count = get_all_files_in_dir("./traces", trace_names, 10);
		for (i = 0; i < traces_count; ++i)
		{
			if ((sscanf(trace_names[i], "%55[^.].%55s", line, input_filename) == 2) && !strcmp(input_filename, "trc"))
			{
				sprintf(input_filename, "./traces/%s", trace_names[i]);
				printf("Trace\t%s:\n", trace_names[i]);
				run_test(input_filename, &cache, &ddr, config, settings);
			}
		}
	}
	
	release_ddr(&ddr);
	release_cache(&cache);
}

int runner(char *trace_f, char *config_f, char *type, unsigned int num_params, unsigned int *params_arr, int settings)
{
	int i, traces_count;
	config config;
	
	read_config(&config, config_f);

	if(type != NULL)
	{
		for (i = 0; i < num_params; ++i)
		{
			sprintf(range_param, "%s = 0x%x", type, params_arr[i]);
			parce_line(&config, range_param);

			run_loop(trace_f, &config, settings);
		}
	}
	else
	{
		run_loop(trace_f, &config, settings);
	}
	
	return 0;
}