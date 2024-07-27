#ifndef DDR_BANK_H
#define DDR_BANK_H

#include "defines.h"

void init_ddr_bank(ddr_bank *bank, config *cfg);
ERROR_RET access_to_ddr_bank(ddr_bank *bank, unsigned int addr, unsigned int size, unsigned int *cost);
void release_ddr_bank(ddr_bank *bank);

#endif // DDR_BANK_H