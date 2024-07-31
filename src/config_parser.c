#include "config_parser.h"
#include "common.h"

static const char *cache_level_params[] = {"size", "sets", "cost"};
static const char *cache_params[] = {"levels"};
static const char *ddr_params[] = {"banks", "row_size", "RAS", "CAS", "interleaving", "channels", "dimms", "ranks", "ch_pos"};
static const char *system_params[] = {"bus", "page_size"};

int get_param_num(const char **arr, unsigned int arr_size, const char *param_name);
int parse_param_line(const char *line, char *param_name, unsigned int *param_value);
int set_cache_level_param(cache_level_config *cfg, char *line);
int set_cache(cache_config *config, char *line);
int set_ddr(ddr_config *config, char *line);

int get_param_num(const char **arr, unsigned int arr_size, const char *param_name)
{
	int idx;
	for(idx = 0; idx < arr_size; ++idx){
		if(!strcmp(param_name, arr[idx]))
		{
			return idx;
		} 
	}

	return -1;
}

int parse_param_line(const char *line, char *param_name, unsigned int *param_value)
{
	unsigned int ret = 0;
	if (sscanf(line, "%s = 0x%x\n", param_name, param_value) != 2)
	{
		ret = -1;
	}

	return ret;
}

int set_cache_level_param(cache_level_config *cfg, char *line)
{
	char param_name[MAX_PARAM_LENGTH];
	unsigned int param_value, param_num;

	if (!parse_param_line(line, param_name, &param_value))
	{
		if ((param_num = get_param_num(cache_level_params, 3, param_name)) == -1)
			return -1;

		switch (param_num)
		{
		case 0:
			if (!is_power_of_2(param_value))
			{
				printf("%s ERROR: %s = 0x%x (mast be power of 2)", __FUNCTION__, param_name, param_value);
				return -1;
			}
			cfg->size = param_value * _1K;
			break;
		case 1:
			if (!is_power_of_2(param_value))
			{
				printf("%s ERROR: %s = 0x%x (mast be power of 2)", __FUNCTION__, param_name, param_value);
				return -1;
			}
			cfg->sets = param_value;
			break;
		case 2:
			cfg->cost = param_value;
			break;
		default:
			printf("%s Error: Wrong parameter: %s\n", __FUNCTION__, param_name);
			break;
		}
	}

	return 0;
}

int set_ddr(ddr_config *cfg, char *line)
{
	char param_name[MAX_PARAM_LENGTH];
	unsigned int param_value, channel;

	if (!parse_param_line(line, param_name, &param_value))
	{
		if ((channel = get_param_num(ddr_params, 9, param_name)) == -1)
			return -1;

		switch (channel)
		{
		case 0:
			if (!is_power_of_2(param_value))
			{
				printf("%s ERROR: %s = 0x%x (mast be power of 2)", __FUNCTION__, param_name, param_value);
				return -1;
			}
			cfg->num_of_banks = param_value;
			break;
		case 1:
			if (!is_power_of_2(param_value))
			{
				printf("%s ERROR: %s = 0x%x (mast be power of 2)", __FUNCTION__, param_name, param_value);
				return -1;
			}
			cfg->row_size = param_value;
			break;
		case 2:
			cfg->RAS = param_value;
			break;
		case 3:
			cfg->CAS = param_value;
			break;
		case 4:
			if (param_value > 1)
			{
				printf("%s ERROR: %s = 0x%x (mast be 0 or 1)", __FUNCTION__, param_name, param_value);
				return -1;
			}
			cfg->IP = param_value;
			break;
		case 5:
			if (!is_power_of_2(param_value))
			{
				printf("%s ERROR: %s = 0x%x (mast be power of 2)", __FUNCTION__, param_name, param_value);
				return -1;
			}
			cfg->num_of_channels = param_value;
			break;
		case 6:
			if (!is_power_of_2(param_value))
			{
				printf("%s ERROR: %s = 0x%x (mast be power of 2)", __FUNCTION__, param_name, param_value);
				return -1;
			}
			cfg->num_of_dimms = param_value;
			break;
		case 7:
			if (!param_value || param_value > 2)
			{
				printf("%s ERROR: %s = 0x%x (mast be 1 or 2)", __FUNCTION__, param_name, param_value);
				return -1;
			}
			cfg->num_of_ranks = param_value;
			break;
		case 8:
			if (param_value > 2)
			{
				printf("%s ERROR: %s = 0x%x (mast be 0, 1 or 2)", __FUNCTION__, param_name, param_value);
				return -1;
			}
			cfg->channel_pos = param_value;
			break;
			default : printf("%s Error: Wrong parameter: %s\n", __FUNCTION__, param_name);
				return -1;
		}
	}

	return 0;
}

int set_cache(cache_config *config, char *line)
{
	char param_name[MAX_PARAM_LENGTH];
	unsigned int param_value, level;
	if (line[0] == 'L' && line[2] == '.')
	{
		level = line[1] - 49;
		if (set_cache_level_param(&(config->cache_configs[level]), &line[3]))
		{
			printf("%s Error: Can't set parameter in Cache level %d '%s'\n", __FUNCTION__, level, &line[3]);
		}
	}
	else
	{
		if (!parse_param_line(line, param_name, &param_value))
		{
			if ((level = get_param_num(cache_params, 1, param_name)) == -1)
				return -1;

			switch (level)
			{
			case 0:
				config->cache_levels = param_value;
				break;
			default:
				printf("%s Error: Wrong parameter: %s\n", __FUNCTION__, param_name);
				return -1;
			}
		 }
	}
	return 0;
}

int parce_line(config *config, char *line)
{
	char param_name[MAX_PARAM_LENGTH];
	unsigned int param_value;
	int param_num, read;
	
	switch (line[0])
	{
	case '#':  // c-sharp style comments
	case '\n': // empty line
		break;
	case 'S': // Top system parameter
		if (line[1] == '.' && !parse_param_line(&line[2], param_name, &param_value))
		{
			param_num = get_param_num(system_params, 2, param_name);
			switch (param_num)
			{
			case 0:
				if (!is_power_of_2(param_value))
				{
					printf("%s ERROR: %s = 0x%x (mast be power of 2)", __FUNCTION__, param_name, param_value);
					return -1;
				}
				config->bus_width = param_value;
				break;
			case 1:
				if (!is_power_of_2(param_value))
				{
					printf("%s ERROR: %s = 0x%x (mast be power of 2)", __FUNCTION__, param_name, param_value);
					return -1;
				}
				config->page_size = param_value;
				break;
			default:
				printf("%s Error: can't find system parameter '%s'\n", __FUNCTION__, param_name);
			}
		}
		break;
	case 'D': // DDR perameter
		if (line[3] == '.')
		{
			if (set_ddr(&(config->ddr_cfg), &line[4]))
			{
				printf("%s Error: can't config parameter '%s' in DDR\n", __FUNCTION__, &line[4]);
			}
		}
		break;
	case 'C': // Cache parameter
		if (line[1] == '.')
		{
			if (set_cache(&(config->cache_cfg), &line[2]))
			{
				printf("%s Error: can't config parameter '%s' in CACHE\n", __FUNCTION__, &line[2]);
			}
		}
		break;
	default: // wrong parameter
		printf("%s Error: Wrong line: %s\n", __FUNCTION__, line);
	}
	return 0;
}

int read_config(config *config, char *config_f)
{
	char line[MAX_LINE_LENGTH];

	if (config_f != NULL)
	{
		sprintf(line, "./%s.trc", config_f);
	}
	else
	{
		memcpy(line, "./default.config", MAX_LINE_LENGTH);
	}
	
		FILE *file = fopen(line, "r");
	if (!file)
	{
		printf("Error: opening config file\n");
		return 1;
	}

	while (fgets(line, MAX_LINE_LENGTH, file))
	{
		parce_line(config, line);
	}

	fclose(file);
	return 0;
}

void print_cfg(config *cfg)
{
	unsigned int i;
	printf("\nCache Simulator configuration:\n");
	printf("Bus width = %u bytes\n", cfg->bus_width);
	printf("Page size = 0x%x bytes\n", cfg->page_size);
	printf("\nCache levels = %u\n", cfg->cache_cfg.cache_levels);
	for (i = 0; i < cfg->cache_cfg.cache_levels; ++i)
	{
		printf("Level %u:\tSize = 0x%x bytes\tSets = %u, HIT cost = %u\n", i
				, cfg->cache_cfg.cache_configs[i].size
				, cfg->cache_cfg.cache_configs[i].sets
				, cfg->cache_cfg.cache_configs[i].cost
				);
	}

	printf("DDR have size 2 GBytes, %u channels, %u dimms, %u ranks and %u banks with row-size %x KBytes.\nCAS = %u, RAS = %u. Interleavingpolicy = %u\n\n"
			, cfg->ddr_cfg.num_of_channels
			, cfg->ddr_cfg.num_of_dimms
			, cfg->ddr_cfg.num_of_ranks
			, cfg->ddr_cfg.num_of_banks
			, cfg->ddr_cfg.row_size / _1K
			, cfg->ddr_cfg.CAS
			, cfg->ddr_cfg.RAS
			, cfg->ddr_cfg.IP
			);
}