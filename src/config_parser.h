#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "defines.h"

int is_power_of_2(int num);
int get_current_dir(char *buff, int size);
int get_all_files_in_dir(char *path, char **names_arr, int arr_size);

int set_param(cache_config* config, int type, int value);

#endif // CONFIG_PARSER_H