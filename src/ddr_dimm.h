#ifndef DDR_DIMM_H
#define DDR_DIMM_H

#include "defines.h"

void init_ddr_dimm(ddr_dimm *dimm, config *cfg);
ERROR_RET access_to_ddr_dimm(ddr_dimm *dimm, unsigned int addr, unsigned int size, unsigned int *cost);
void release_ddr_dimm(ddr_dimm *dimm);

#endif // DDR_DIMM_H