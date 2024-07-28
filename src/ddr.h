#ifndef DDR_H
#define DDR_H

#include "defines.h"

void init_ddr(ddr *ddr, config *cfg);
DDR_RET access_to_ddr(ddr *ddr, unsigned int addr);
void release_ddr(ddr *ddr);

#endif // DDR_H