#ifndef DDR_RANK_H
#define DDR_RANK_H

#include "defines.h"

void init_ddr_rank(ddr_rank *rank, config *cfg);
ERROR_RET access_to_ddr_rank(ddr_rank *rank, unsigned int addr, unsigned int size, unsigned int *cost);
void release_ddr_rank(ddr_rank *rank);

#endif // DDR_RANK_H