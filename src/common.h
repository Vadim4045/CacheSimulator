#ifndef COMMON_H
#define COMMON_H

#include "defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *ret2str(RET_STATUS ret);
int log2int(unsigned int n);
BOOL is_power_of_2(unsigned int num);
int get_all_files_in_dir(char *path, char **names_arr, int arr_size);
void replace_simbol(char *str, char from, char to);

#endif // COMMON_H
