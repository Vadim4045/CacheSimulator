#ifndef DDR_BANK_H
#define DDR_BANK_H

#include "defines.h"

void init_ddr_bank
	(ddr_bank *bank, ddr_bank_config *cfg, unsigned int banks, unsigned int size, unsigned int page_size, unsigned int bus_width);
unsigned int access_to_bank(ddr_bank *bank, unsigned int *addr);

#endif // DDR_BANK_H