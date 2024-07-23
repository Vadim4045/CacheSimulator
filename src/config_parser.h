#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "defines.h"

int is_power_of_2(int num);
int get_all_files_in_dir(char *path, char **names_arr, int arr_size);
int get_param_num(const char **arr, uchar arr_size, const char *param_name);
int parse_param_line(const char *line, char *param_name, int *param_value, char simbol);
int set_ddr_channel_param(ddr_channel_config *cfg, char *line);
int set_cache_level_param(cache_level_config *cfg, char *line);
int set_ddr(ddr_config *config, char *line);
int set_cache(cache_config *config, char *line);
int init_simulator(config *config, char *config_f);
int set_param(config *config, int type, int value);

#endif // CONFIG_PARSER_H