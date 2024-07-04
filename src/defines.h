#ifndef DEFINES_H
#define DEFINES_H

#define SYS_BUS 4

#define MASK(bits) ((1 << (bits)) - 1)
#define PART_NUM(num, pos, bits) (((num) >> (pos)) & MASK(bits))

#ifdef DEBUG_MODE
#define DEBUG(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define DEBUG(fmt, ...)
#endif

#define uint unsigned int
#define ulong unsigned long
#define ull unsigned long long

#define _1K (1 << 10)
#define _2K (2 * (1 << 10))
#define _4K (4 * (1 << 10))
#define _8K (8 * (1 << 10))
#define _16K (16 * (1 << 10))
#define _32K (32 * (1 << 10))
#define _64K (64 * (1 << 10))
#define _128K (128 * (1 << 10))
#define _256K (256 * (1 << 10))
#define _512K (512 * (1 << 10))
#define _1M (1 << 20)
#define _2M (2 * (1 << 20))
#define _4M (4 * (1 << 20))
#define _8M (8 * (1 << 20))
#define _16M (16 * (1 << 20))
#define _32M (32 * (1 << 20))
#define _64M (64 * (1 << 20))
#define _128M (128 * (1 << 20))
#define _256M (256 * (1 << 20))
#define _512M (512 * (1 << 20))

typedef enum
{
	False = 0,
	True = 1
} BOOL;

typedef enum
{
	HIT,
	MISS,
	WRITE_BACK
} RET_STATUS;

typedef enum
{
	WRITE_ALLOCATE,
	WRITE_AROUND
} WRITE_POLICY;

typedef enum
{
	FIFO,
	LRU,
	LFU,
} REPLAСEMENT_POLICY;

typedef enum
{
	DIRECT_MAPPED,
	SET_ASSOCIATIVE,
	FULLY_ASSOCIATIVE
} ASSOCIATIVITY;
typedef struct
{
	unsigned char level;
	unsigned char page_size;
	unsigned char total_size;

	WRITE_POLICY WP;
	REPLAСEMENT_POLICY RP;
	ASSOCIATIVITY AC;

	unsigned char sets;
} cache_level_config;

typedef enum
{
	ROW_INTERLEAVING,
	BLOCK_INTERLEAVING
} INTERLEAVING_POLICY;

typedef struct
{
	unsigned char bus_width;
	unsigned char channels;
	unsigned char dimms;
	unsigned char banks;

	INTERLEAVING_POLICY IP;
} ddr_config;

typedef struct _cache_config
{
	unsigned char cache_levels;
	cache_level_config *cache_configurations;
	ddr_config ddr_configuration;

	struct _cache_config *next;
} cache_config;

/**
 * _tag_width bits - TAG
 *  1 bit - is valid
 *  1 bit - is writed
 *  _counter_width - LRU counter
 */
typedef struct
{
	unsigned int _bank_size;
	unsigned int _page_size;
	unsigned int _pages;
	unsigned int _offcet_width;
	unsigned int _tag_width;
	unsigned int _set_width;
	unsigned int _counter_width;
	// unsigned char* pages_arr;
	unsigned int *addr_arr;

} cache_level;

#endif // DEFINES_H