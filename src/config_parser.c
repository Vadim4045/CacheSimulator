#include "config_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

static const char *cache_level_params[] = {"size", "sets"};
static const char *ddr_channel_params[] = {"dimms", "banks"};
static const char *top_ddr_params[] = {"channels", "interleaving"};
static const char *top_cache_params[] = {"bus", "page_size", "levels", "write_policy", "replaycement", "associativity"};

int is_power_of_2(int num)
{
	while (num & 1 == 0)
	{
		num >>= 1;
	}
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

int get_param_num(const char** arr, uchar arr_size, const char* param_name){
	int idx;
	for(idx = 0; idx < arr_size; ++idx){
		if(!strcmp(param_name, arr[idx])) return idx;
	}
	return -1;
}

int parse_param_line(const char *line, char *param_name, int *param_value, char simbol)
{
	const char *equals_sign = strchr(line, simbol);
	if (equals_sign == NULL)
	{
		return -1;
	}

	size_t name_length = equals_sign - line;
	if (name_length == 0)
	{
		return -1;
	}

	strncpy(param_name, line, name_length);
	param_name[name_length] = '\0';

	const char *value_str = equals_sign + 1;
	*param_value = atoi(value_str);

	return !is_power_of_2(*param_value);
}

int set_ddr_channel_param(ddr_channel_config *channel_cfg, char *param, int val)
{

	return 0;
}

int set_ddr_param(ddr_config* ddr_cfg, char* param, int val){
	
	
	return 0;
}

int set_cache_level_param(cache_level_config *cl_cfg, char *param, int val) 
{
	int idx;

	if ((idx = get_param_num(cache_level_params, 2, param)) == -1)
		return -1;

	switch (idx)
	{
	case 0:
		cl_cfg->size = val;
		break;
	case 1:
		cl_cfg->sets = val;
		break;
	default:
		break;
	}
	return 0;
}

int set_cache_param(cache_config *cache_cfg, char *param, int val)
{
	int idx;

	if ((idx = get_param_num(top_cache_params, 6, param)) == -1)
		return -1;

	switch (idx)
	{
	case 0:
		cache_cfg->bus_width = val;
		break;
	case 1:
		cache_cfg->page_size = val;
		break;
	case 2:
		cache_cfg->cache_levels = val;
		break;
	case 3:
		cache_cfg->WP = val;
		break;
	case 4:
		cache_cfg->RP = val;
		break;
	case 5:
		cache_cfg->AC = val;
		break;
	default:
		break;
	}
	return 0;
}

int init_simulator(cache_config *config, char *config_f)
{
	char line[MAX_LINE_LENGTH];
	char param_name[MAX_PARAM_LENGTH];
	int param_value, param_num;
	
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

	while (fgets(line, sizeof(line), file))
	{
		switch(line[0])
		{
			case '#': // c-sharp style comments
			case '\n': // empty line
				break;
			case 'L': // cache levels config (L1.param=val)
				if (line[2] == '.' && !parse_param_line(&line[3], param_name, &param_value, '=')){
					param_num = line[1] - 48;
					if (param_num > 0 && param_num <= 3){
						if (set_cache_level_param(&config->cache_configurations[param_num - 1], param_name, param_value))
						{
							printf("Error: can't config parameter $s in L%d cache\n", param_name, param_num);
						}
					}else
					{
						printf("Error: There are not more, then 3 cache levels accepted\n");
					}	
				}
				break;
			case 'D': // Top DDR config(DDR.param=val)
				if (line[3] == '.' && !parse_param_line(&line[5], param_name, &param_value, '=')){
					if (set_ddr_param(&config->ddr_configuration, param_name, param_value))
					{
						printf("Error: can't config parameter $s in DDR\n", param_name);
					}
				}
					break;
			case 'C': // DDR channel config (CHANNEL3.param=value)
				if (line[8] == '.' && !parse_param_line(&line[9], param_name, &param_value, '='))
				{
					param_num = line[7] - 48;
					if (param_num > 0 && param_num <= 4){
						if (set_ddr_channel_param(&config->ddr_configuration.channels_config[param_num], param_name, param_value))
						{
							printf("Error: can't config parameter $s in DDR%d channel\n", param_name, param_num);
						}
					}
					else
					{
						printf("Error: There are not more, then 4 ddr channels accepted\n");
					}
				}
				break;
			default: // Top cache parameters (param=value)
				if (!parse_param_line(&line[9], param_name, &param_value, '=')){
					if (set_cache_param(config, param_name, param_value))
					{
						printf("Error: can't config parameter $s in top cache\n", param_name, param_num);
					}
				}else
				{
					printf("Error: Wrong line: %s\n", line);
				}
		}
	}

	fclose(file);
	return 0;
}
	
// Set specific params to range
int set_param(cache_config *config, int type, int value)
{
	switch(type)
	{
	case 0: // page_size
		config->page_size = value;
		break;
	case 1: // cache_levels
		config->cache_levels = value;
		break;
	case 2: // level1_size
		if (config->cache_levels >= 1)
		{
			config->cache_configurations[0].size = value;
		}
		else
		{
			printf("Error: no have L1");
			return 1;
		}
		break;
	case 3: // level2_size
		if (config->cache_levels >= 2)
		{
			config->cache_configurations[1].size = value;
		}
		else
		{
			printf("Error: no have L2");
			return 1;
		}
		break;
	case 4: // level3_size
		if (config->cache_levels >= 3)
		{
			config->cache_configurations[2].size = value;
		}
		else
		{
			printf("Error: no have L3");
			return 1;
		}
		break;
	case 5: // write_policy
		if (value < WRONG_POLICY)
		{
			config->WP = value;
		}
		else
		{
			printf("Error: wrong write policy");
			return 1;
		}
		break;
	case 6: // replaсement
		if (value < WRONG_REPLAСEMENT)
		{
			config->RP = value;
		}
		else
		{
			printf("Error: wrong replaсement");
			return 1;
		}
		break;
	case 7: // associativity
		if (value < WRONG_ASSOCIATIVITY)
		{
			config->AC = value;
		}
		else
		{
			printf("Error: wrong associativity");
			return 1;
		}
		break;
	case 8: // interleaving
		if (value < WRONG_INTERLEAVING)
		{
			config->ddr_configuration.IP = value;
		}
		else
		{
			printf("Error: wrong interleaving");
			return 1;
		}
		break;
	default:
		printf("Error: wrong type of parameter");
		return 1;
	}
	return 0;
}