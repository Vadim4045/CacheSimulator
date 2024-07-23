#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "defines.h"

int is_power_of_2(int num);
int do_instruction(char *instruction_string, uint addr, uint *cost);
int run_test(char *trace_f, config *config);
int runner(char *trace_f, char *config_f, int type, uint num_params, uint *params_arr);

#endif // TEST_RUNNER_H