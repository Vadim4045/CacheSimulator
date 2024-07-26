#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

char * ret_values[] = {"HIT", "MISS", "WRITE_BACK", "RET_WRONG"};

char * ret2str(RET_STATUS ret)
{
	return ret_values[ret];
}

int is_power_of_2(int num)
{
	if (num == 0)
		return 1;

	while ((num & 1) == 0)
		num >>= 1;
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