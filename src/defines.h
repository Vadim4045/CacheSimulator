#ifndef DEFINES_H
#define DEFINES_H

#define MAX_PARAM_LENGTH 56
#define MAX_LINE_LENGTH 1024
#define MASK(bits) ((1 << (bits)) - 1)
#define PART_NUM(num, pos, bits) (((num) >> (pos)) & MASK(bits))

#ifdef DEBUG_MODE
#define DEBUG(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define DEBUG(fmt, ...)
#endif

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
#define ull unsigned long long

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
	uint size;
	uint sets;
} cache_level_config;

typedef struct
{
	uint cache_levels;
	WRITE_POLICY WP;
	REPLAСEMENT_POLICY RP;
	ASSOCIATIVITY AC;
	cache_level_config cache_configs[3];
} cache_config;

typedef struct
{
	uint dimms;
	uint banks;
} ddr_channel_config;

typedef struct
{
	uint size;
	uint channels;
	INTERLEAVING_POLICY IP;

	ddr_channel_config channels_config[4];
} ddr_config;

typedef struct
{
	uint bus_width;
	uint page_size;
	
	cache_config cache_cfg;
	ddr_config ddr_cfg;
} config;

/**
 * _tag_width bits - TAG
 *  1 bit - is valid
 *  1 bit - is writed
 *  _counter_width - LRU counter
 */
typedef struct
{
	uint _pages;
	uint _offcet_width;
	uint _tag_width;
	uint _set_width;
	uint _counter_width;

	unsigned int *addr_arr;
} cache_set;

typedef struct
{
	uint _bank_size;
	uint _page_size;
	uint _sets;
	
	// uchar* pages_arr;
	cache_set *sets_arr;
	uint* sets_status;

} cache_level;

typedef struct
{
	uint _cache_levels_num;
	cache_level _cache_levels_inst[3];

} cache;

typedef struct
{
	cache cache;
} cache_simulator;

#pragma pack(pop)
#endif // DEFINES_H