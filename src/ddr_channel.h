#ifndef DDR_CHANNEL_H
#define DDR_CHANNEL_H

#include "defines.h"

void init_ddr_channel(ddr_channel *channel, config *cfg);
ERROR_RET access_to_ddr_channel(ddr_channel *channel, unsigned int addr, unsigned int size, unsigned int *cost);
void release_ddr_channel(ddr_channel *channel);

#endif // DDR_CHANNEL_H