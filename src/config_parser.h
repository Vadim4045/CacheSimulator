#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "defines.h"

int get_param_num(const char **arr, uint arr_size, const char *param_name);
int parse_param_line(const char *line, char *param_name, int *param_value);
int set_ddr_channel_param(ddr_channel_config *cfg, char *line);
int set_cache_level_param(cache_level_config *cfg, char *line);
int set_ddr(ddr_config *config, char *line);
int set_cache(cache_config *config, char *line);
int read_config(config *config, char *config_f);
void print_cfg(config* cfg);

#endif // CONFIG_PARSER_H