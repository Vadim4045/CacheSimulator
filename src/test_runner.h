#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "defines.h"

int is_power_of_2(int num);
RET_STATUS do_instruction(cache* cache, char *instruction_string, unsigned int addr, unsigned int *cost);
int run_test(char *trace_f, config *config);
int runner(char *trace_f, char *config_f);

#endif // TEST_RUNNER_H