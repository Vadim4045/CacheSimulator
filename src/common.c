#include "common.h"
#include <dirent.h>

char * ret_values[] = {"HIT", "MISS", "WRITE_BACK", "RET_WRONG"};

char * ret2str(RET_STATUS ret)
{
	return ret_values[ret];
}

int log2int(unsigned int n)
{ // but truncated
	if (n == 0) return 0; // not math
	
	int logValue = -1;
	while (n)
	{ //
		logValue++;
		n >>= 1;
	}
	return logValue;
}

BOOL is_power_of_2(unsigned int num)
{
	if (num < 2) return 1;

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

void replace_simbol(char *str, char from, char to)
{
	while (*str != '\0')
	{
		if (*str == from)
		{
			*str = to;
		}
		str++;
	}
}