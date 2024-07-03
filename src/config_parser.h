#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#define _1K (1 << 10)
#define _1M (1 << 20)

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
} REPLASEMENT_POLICY;

typedef enum
{
	DIRECT_MAPPED,
	SET_ASSOCIATIVE,
	FULLY_ASSOCIATIVE
} ASSOCIATIVITY;
typedef struct
{
	unsigned char 		level;
	unsigned char 		page_size;
	unsigned char 		total_size;

	WRITE_POLICY 		WP;
	REPLASEMENT_POLICY 	RP;
	ASSOCIATIVITY 		AC;

	unsigned char 		sets;
} cache_level_config;



typedef enum
{
	ROW_INTERLEAVING,
	BLOCK_INTERLEAVING
} INTERLEAVING_POLICY;

typedef struct
{
	unsigned char 		bus_width;
	unsigned char 		channels;
	unsigned char 		dimms;
	unsigned char 		banks;

	INTERLEAVING_POLICY IP;
} ddr_config;

typedef struct _cache_config
{
	unsigned char 			cache_levels;
	cache_level_config*		cache_configurations;
	ddr_config 				ddr_configuration;
	
	_cache_config* 			next;
} cache_config;


#endif // CONFIG_PARSER_H