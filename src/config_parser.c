#include "config_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

int is_power_of_2(int num)
{
	while (num & 1 == 0)
	{
		num >>= 1;
	}
	return num == 1;
}

int get_current_dir(char *buff, int size)
{
	char exe_path[size];

#ifdef __linux__
	ssize_t len = readlink("/proc/self/exe", exe_path, size - 1);
	if (len != -1)
	{
		exe_path[len] = '\0';
	}
	else
	{
		perror("readlink");
		return 1;
	}
#elif __APPLE__
	uint32_t bufsize = size;
	if (_NSGetExecutablePath(exe_path, &bufsize))
	{
		fprintf(stderr, "Buffer too small; need size %u\n", bufsize);
		return 1;
	}
#elif _WIN32
	DWORD len = GetModuleFileName(NULL, exe_path, size);
	if (len == 0)
	{
		fprintf(stderr, "GetModuleFileName failed with error %lu\n", GetLastError());
		return 1;
	}
#else
	fprintf(stderr, "Unsupported platform\n");
	return 1;
#endif

	strncpy(buff, dirname(exe_path), size);
	return 0;
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

int set_param(cache_config* config, int type, int value)
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