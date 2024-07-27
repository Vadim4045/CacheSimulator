#ifndef DEFINES_H
#define DEFINES_H

#define MAX_PARAM_LENGTH 56
#define MAX_LINE_LENGTH 1024

#define GET_MASK(bits) ((1 << (bits)) - 1)
#define GET_FIELD(x, pos, width) (((x) >> (pos)) & GET_MASK(width))
#define CLEAR_FIELD(x, pos, width)    ((x) &= ~(GET_MASK(width) << (pos)))
#define SET_FIELD(x, pos, width, val) ((x) = ((x) & ~(GET_MASK(width) << (pos))) | (((val) & GET_MASK(width)) << (pos)))

#define _1K (1 << 10)
#define _1M (1 << 20)

typedef enum
{
	False = 0,
	True = 1,
	Wrong
} BOOL;

typedef enum
{
	HIT,
	MISS,
	WRITE_BACK,
	RET_WRONG
} RET_STATUS;

typedef enum
{
	WRITE_ALLOCATE,
	WRITE_AROUND,
	WRONG_POLICY
} WRITE_POLICY;

typedef enum
{
	FIFO,
	LRU,
	LFU,
	WRONG_REPLAСEMENT
} REPLAСEMENT_POLICY;

typedef enum
{
	DIRECT_MAPPED,
	SET_ASSOCIATIVE,
	FULLY_ASSOCIATIVE,
	WRONG_ASSOCIATIVITY
} ASSOCIATIVITY;

typedef enum
{
	ROW_INTERLEAVING,
	BLOCK_INTERLEAVING,
	WRONG_INTERLEAVING
} INTERLEAVING_POLICY;

#pragma pack(push, 4)
typedef struct
{
	unsigned int size;
	unsigned int sets;
	unsigned int cost;
} cache_level_config;

typedef struct
{
	unsigned int cache_levels;
	WRITE_POLICY WP;
	REPLAСEMENT_POLICY RP;
	ASSOCIATIVITY AC;
	cache_level_config cache_configs[3];
} cache_config;

typedef struct
{
	unsigned int size;
	unsigned int dimms;
	unsigned int banks;
	unsigned int RAS;
	unsigned int CAS;
} ddr_channel_config;

typedef struct
{
	unsigned int channels;
	INTERLEAVING_POLICY IP;

	ddr_channel_config channels_config[4];
} ddr_config;

typedef struct
{
	unsigned int bus_width;
	unsigned int page_size;
	
	cache_config cache_cfg;
	ddr_config ddr_cfg;
} config;

/**
 *  1 bit - is valid
 *  1 bit - is writed
 *  _counter_width - LRU counter
 *   _tag_width bits - TAG
 */
typedef struct
{
	unsigned int _pages;
	unsigned int _offcet_width;
	unsigned int _tag_width;
	unsigned int _set_width;
	unsigned int _counter_width;

	unsigned int *addr_arr;
} cache_set;

typedef struct
{
	unsigned int level;
	unsigned int _size;
	unsigned int _page_size;
	unsigned int _sets;
	unsigned int _hit_cost;
	
	// uchar* pages_arr;
	cache_set *sets_arr;
	unsigned int* sets_status;

} cache_level;

typedef struct
{
	unsigned int _bus_width;
	unsigned int _cache_levels_num;
	cache_level _cache_levels_inst[3];

} cache;

typedef struct
{
	cache cache;
} cache_simulator;

#pragma pack(pop)
#endif // DEFINES_H