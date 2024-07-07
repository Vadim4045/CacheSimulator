#include "config_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>


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

int set_ddr_param(ddr_config* ddr_cfg, char* param, int val){
	
	
	return 0;
}

int set_cache_level_param(cache_level_config *cl_cfg, char *param, int val) 
{

	return 0;
}

int set_cache_param(cache_config *cache_cfg, char *param, int val)
{

	return 0;
}

int init_simulator(cache_config *config, char *config_f)
{
	char line[MAX_LINE_LENGTH];
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
			case '#':
				break;
			case 'L':
				switch (line[1])
				{
					case '1':
					
						break;
					case '2':
					
						break;
					case '3':
					
						break;
					default:
						
				}
				break;
			case 'D':

				break;
			case 'C':

				break;
			default:
		}
	}

	fclose(file);
	return 0;
}
	
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
			config->cache_configurations[0].total_size = value;
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
			config->cache_configurations[1].total_size = value;
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
			config->cache_configurations[2].total_size = value;
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