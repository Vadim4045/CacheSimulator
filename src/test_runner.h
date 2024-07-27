#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "defines.h"

int runner(char *trace_f, char *config_f, char *type, unsigned int num_params, unsigned int *params_arr, int settings);

#endif // TEST_RUNNER_H