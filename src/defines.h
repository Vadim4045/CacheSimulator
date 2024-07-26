#ifndef DEFINES_H
#define DEFINES_H

#define MAX_PARAM_LENGTH 56
#define MAX_LINE_LENGTH 1024

#define GET_MASK(bits) ((1 << (bits)) - 1)
#define GET_FIELD(x, pos, width) (((x) >> (pos)) & GET_MASK(width))
#define CLEAR_FIELD(x, pos, width)    ((x) &= ~(GET_MASK(width) << (pos)))
#define SET_FIELD(x, pos, width, val) ((x) = ((x) & ~(GET_MASK(width) << (pos))) | ((val) << (pos)))

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
	uint cost;
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
	uint size;
	uint dimms;
	uint banks;
	uint RAS;
	uint CAS;
} ddr_channel_config;

typedef struct
{
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
 *  1 bit - is valid
 *  1 bit - is writed
 *  _counter_width - LRU counter
 *   _tag_width bits - TAG
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
	uint level;
	uint _size;
	uint _page_size;
	uint _sets;
	uint _hit_cost;
	
	// uchar* pages_arr;
	cache_set *sets_arr;
	uint* sets_status;

} cache_level;

typedef struct
{
	uint _bus_width;
	uint _cache_levels_num;
	cache_level _cache_levels_inst[3];

} cache;

typedef struct
{
	cache cache;
} cache_simulator;

#pragma pack(pop)
#endif // DEFINES_H