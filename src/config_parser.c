#include "config_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

static const char *cache_level_params[] = {"size", "sets"};
static const char *cache_params[] = {"levels", "write_policy", "replaycement", "associativity"};
static const char *ddr_channel_params[] = {"dimms", "banks"};
static const char *ddr_params[] = {"size", "channels", "interleaving"};
static const char *system_params[] = {"bus", "page_size"};

int is_power_of_2(int num)
{
	while ((num & 1) == 0) num >>= 1;
	return num == 1;
}

int get_all_files_in_dir(char *path, char **names_arr, int arr_size)
{
	DIR *dir;
	struct dirent *entry;
	int count = 0;

	if ((dir = opendir(path)) == NULL)
	{
		perror("opendir() error");
		return -1;
	}
	else
	{
		while ((entry = readdir(dir)) != NULL && count < arr_size)
		{
			if (entry->d_type == DT_REG)
			{
				names_arr[count] = malloc(strlen(entry->d_name) + 1);
				if (names_arr[count] == NULL)
				{
					perror("malloc() error");
					closedir(dir);
					return -1;
				}
				strcpy(names_arr[count], entry->d_name);
				++count;
			}
		}
		closedir(dir);
	}
	return count;
}

int get_param_num(const char **arr, uint arr_size, const char *param_name)
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

int parse_param_line(const char *line, char *param_name, int *param_value)
{
	uint ret = 0;
	if (sscanf(line, "%s = 0x%x\n", param_name, param_value) != 2)
	{
		ret = -1;
	}

	return ret;
}

int set_ddr_channel_param(ddr_channel_config *cfg, char *line) 
{
	char param_name[MAX_PARAM_LENGTH];
	uint param_value, param_num;

	if (!parse_param_line(line, param_name, &param_value))
	{
		if ((param_num = get_param_num(ddr_channel_params, 2, param_name)) == -1)
			return -1;

		switch (param_num)
		{
		case 0:
			if (!is_power_of_2(param_value))
			{
				printf("%s ERROR: %s = 0x%x (mast be power of 2)", __FUNCTION__, param_name, param_value);
				return -1;
			}
			cfg->dimms = param_value;
			break;
		case 1:
			if (!is_power_of_2(param_value))
			{
				printf("%s ERROR: %s = 0x%x (mast be power of 2)", __FUNCTION__, param_name, param_value);
				return -1;
			}
			cfg->banks = param_value;
			break;
		default:
			break;
		}
	}
	
	return 0;
}

int set_cache_level_param(cache_level_config *cfg, char *line)
{
	char param_name[MAX_PARAM_LENGTH];
	uint param_value, param_num;

	if (!parse_param_line(line, param_name, &param_value))
	{
		if ((param_num = get_param_num(cache_level_params, 2, param_name)) == -1)
			return -1;

		switch (param_num)
		{
		case 0:
			if(!is_power_of_2(param_value)){
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
		default:
			break;
		}
	}

	return 0;
}

int set_ddr(ddr_config *config, char *line)
{
	char param_name[MAX_PARAM_LENGTH];
	uint param_value, channel;
	if (line[0] == 'C' && line[3] == '.')
	{
		channel = line[2] - 48;
		if (set_ddr_channel_param(&(config->channels_config[channel]), &line[4]))
		{
			printf("%s Error: Can't set parameter in DDR channel %d '%s'\n", __FUNCTION__, channel, &line[4]);
		}
	}
	else
	{
		if (!parse_param_line(line, param_name, &param_value))
		{
			if ((channel = get_param_num(ddr_params, 3, param_name)) == -1)
				return -1;

			switch (channel)
			{
			case 0:
				if (!is_power_of_2(param_value))
				{
					printf("%s ERROR: %s = 0x%x (mast be power of 2)", __FUNCTION__, param_name, param_value);
					return -1;
				}
				config->size = param_value * _1M;
				break;
			case 1:
				config->channels = param_value;
				break;
			case 2:
				config->IP = param_value;
				break;
			default:
				return -1;
			}
		}
	}
	return 0;
}

int set_cache(cache_config *config, char *line)
{
	char param_name[MAX_PARAM_LENGTH];
	uint param_value, level;
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
			if ((level = get_param_num(cache_params, 4, param_name)) == -1)
				return -1;

			switch (level)
			{
			case 0:
				config->cache_levels = param_value;
				break;
			case 1:
				config->WP = param_value;
				break;
			case 2:
				config->RP = param_value;
				break;
			case 3:
				config->AC = param_value;
				break;
			default:
				printf("%s Error: Wrong parameter: %s\n", __FUNCTION__, param_name);
				return -1;
			}
		 }
	}
	return 0;
}

int read_config(config *config, char *config_f)
{
	char line[MAX_LINE_LENGTH];
	char param_name[MAX_PARAM_LENGTH];
	int param_value, param_num, read;
	
	if (config_f != NULL)
	{
		sprintf(line, "./%s.config", config_f);
	}else{
		sprintf(line, "./default_config.config");
	}

	FILE *file = fopen(line, "r");
	if (!file)
	{
		printf("Error: opening config file\n");
		return 1;
	}

	while (fgets(line, MAX_LINE_LENGTH, file))
	{
		switch (line[0])
		{
		case '#': // c-sharp style comments
		case '\n': // empty line
			break;
		case 'S': // Top system parameter
			if (line[1] == '.' && !parse_param_line(&line[2], param_name, &param_value))
			{
				param_num = get_param_num(system_params, 2, param_name) ;
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
	}

	if (feof(file))
	{
		DEBUG("%s End of file reached.\n", __FUNCTION__);
	}
	else if (ferror(file))
	{
		DEBUG("%s An error occurred.\n", __FUNCTION__);
	}
	
	fclose(file);
	return 0;
}

void print_cfg(config *cfg)
{
	uint i;
	printf("\nCache Simulator configuration:\n");
	printf("Bus width = %d bytes\n", cfg->bus_width);
	printf("Page size = %d bytes\n", cfg->page_size);
	printf("\nCache levels = %d\n", cfg->cache_cfg.cache_levels);
	for (i = 0; i < cfg->cache_cfg.cache_levels; ++i)
	{
		printf("Level %d:\tSize = 0x%x bytes\tSets = %d\n", i
				, cfg->cache_cfg.cache_configs[i].size
				, cfg->cache_cfg.cache_configs[i].sets);
	}
	printf("WP = %d, RP = %d, AC = %d,\n", cfg->cache_cfg.WP, cfg->cache_cfg.RP, cfg->cache_cfg.AC);
	
	printf("DDR total size = 0x%x bytes\n", cfg->ddr_cfg.size);
	printf("DDR have %d channels\n", cfg->ddr_cfg.channels);
	for (i = 0; i < cfg->ddr_cfg.channels; ++i)
	{
		printf("DDR channel %d have %d dimms with %d banks in dimm\n", i
				, cfg->ddr_cfg.channels_config[i].dimms
				, cfg->ddr_cfg.channels_config[i].banks);
	}
	printf("DDR IP = %d\n\n", cfg->ddr_cfg.IP);
}