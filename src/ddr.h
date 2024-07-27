#ifndef DDR_H
#define DDR_H

#include "defines.h"

void init_ddr(ddr *ddr, config *cfg);
ERROR_RET access_to_ddr(ddr *ddr, unsigned int addr, unsigned int size, unsigned int *cost);
void release_ddr(ddr *ddr);

#endif // DDR_H