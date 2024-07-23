#include "config_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

static const char *cache_level_params[] = {"size", "sets"};
static const char *cache_params[] = {"levels", "write_policy", "replaycement", "associativity"};
static const char *ddr_channel_params[] = {"dimms", "banks"};
static const char *ddr_params[] = {"channels", "interleaving"};
static const char *system_params[] = {"bus", "page_size"};

int is_power_of_2(int num)
{
	while (num & 1 == 0) num >>= 1;
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

int get_param_num(const char** arr, uchar arr_size, const char* param_name)
{
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

int set_ddr_channel_param(ddr_channel_config *cfg, char *line) 
{
	DEBUG("ConfigParser::%s\n", __FUNCTION__);
	char param_name[MAX_PARAM_LENGTH];
	uint param_value, param_num;

	if (!parse_param_line(line, param_name, &param_value, '='))
	{
		if ((param_num = get_param_num(ddr_channel_params, 2, param_name)) == -1)
			return -1;

		switch (param_num)
		{
		case 0:
			cfg->dimms = param_value;
			break;
		case 1:
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
	DEBUG("ConfigParser::%s\n", __FUNCTION__);
	char param_name[MAX_PARAM_LENGTH];
	uint param_value, param_num;

	if (!parse_param_line(line, param_name, &param_value, '='))
	{
		if ((param_num = get_param_num(cache_level_params, 2, param_name)) == -1)
			return -1;

		switch (param_num)
		{
		case 0:
			cfg->size = param_value;
			break;
		case 1:
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
	DEBUG("ConfigParser::%s\n", __FUNCTION__);
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
		if (!parse_param_line(&line[2], param_name, &param_value, '='))
		{

			if ((channel = get_param_num(cache_params, 2, param_name)) == -1)
				return -1;

			switch (channel)
			{
			case 0:
				config->channels = param_value;
				break;
			case 1:
				config->IP = param_value;
				break;
			default:
				printf("%s Error: Wrong parameter: %s\n", __FUNCTION__, param_name);
				return -1;
			}
		}
	}
	return 0;
}

int set_cache(cache_config *config, char *line)
{
	DEBUG("ConfigParser::%s\n", __FUNCTION__);
	char param_name[MAX_PARAM_LENGTH];
	uint param_value, level;
	if (line[0] == 'L' && line[2] == '.')
	{
		level = line[1] - 48;
		if (set_cache_level_param(&(config->cache_configs[level]), &line[3]))
		{
			printf("%s Error: Can't set parameter in Cache level %d '%s'\n", __FUNCTION__, level, &line[3]);
		}
	}
	else
	{
		 if(!parse_param_line(line, param_name, &param_value, '='))
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

int init_simulator(config *config, char *config_f)
{
	DEBUG("%s\n", __FUNCTION__);
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
			case 'S': // Top system parameter
				if (line[1] == '.' && !parse_param_line(&line[2], param_name, &param_value, '='))
				{
					param_num = get_param_num(system_params, 2, param_name) ;
					switch (param_num)
					{
					case 0:
						config->bus_width = param_value;
						break;
					case 1:
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

	fclose(file);
	return 0;
}
	
// Set specific params to range
int set_param(config *config, int type, int value)
{
	DEBUG("%s\n", __FUNCTION__);
	switch(type)
	{
	case 0: // page_size
		config->page_size = value;
		break;
	case 1: // cache_levels
		config->cache_cfg.cache_levels = value;
		break;
	case 2: // level1_size
		if (config->cache_cfg.cache_levels >= 1)
		{
			config->cache_cfg.cache_configs[0].size = value;
		}
		else
		{
			printf("Error: no have L1");
			return 1;
		}
		break;
	case 3: // level2_size
		if (config->cache_cfg.cache_levels >= 2)
		{
			config->cache_cfg.cache_configs[1].size = value;
		}
		else
		{
			printf("Error: no have L2");
			return 1;
		}
		break;
	case 4: // level3_size
		if (config->cache_cfg.cache_levels >= 3)
		{
			config->cache_cfg.cache_configs[2].size = value;
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
			config->cache_cfg.WP = value;
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
			config->cache_cfg.RP = value;
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
			config->cache_cfg.AC = value;
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
			config->ddr_cfg.IP = value;
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