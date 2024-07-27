#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "defines.h"

int parce_line(config *config, char *line);
int read_config(config *config, char *config_f);
void print_cfg(config* cfg);

#endif // CONFIG_PARSER_H