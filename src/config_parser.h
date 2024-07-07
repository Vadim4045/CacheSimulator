#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "defines.h"

int get_all_files_in_dir(char *path, char **names_arr, int arr_size);
int set_ddr_param(ddr_config *ddr_cfg, char *param, int val);
int set_cache_level_param(cache_level_config *cl_cfg, char *param, int val);
int set_cache_param(cache_config *cache_cfg, char *param, int val);
int init_simulator(cache_config *config, char *config_f);
int set_param(cache_config *config, int type, int value);

#endif // CONFIG_PARSER_H