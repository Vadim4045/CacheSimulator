#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "defines.h"

int run_test(char *trace, cache_config *config);
int runner(char *trace, int type, uint num_params, uint *params_arr);

#endif // TEST_RUNNER_H