#ifndef CACHE_SIMULATOR_H
#define CACHE_SIMULATOR_H

#include "defines.h"

void cache_init(cache *cache, uint levels_num, uint bank_size, uint page_size, uint counter_width);

RET_STATUS cache_read(cache *cache, uint addr, uint *cost);
RET_STATUS cache_write(cache *cache, uint addr, uint *cost);

#endif // CACHE_SIMULATOR_H