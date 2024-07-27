#include "ddr.h"
#include "common.h"

void init_ddr(ddr *ddr, config *cfg)
{
	unsigned int i;
	ddr->bus = cfg->bus_width;
	ddr->size = cfg->ddr_cfg.size;
	ddr->num_of_channels = cfg->ddr_cfg.num_of_channels;
	ddr->page_size = cfg->page_size;
	ddr->num_of_dimms = cfg->ddr_cfg.channels.num_of_dimms;

	ddr->channels = (ddr_channel *)calloc(ddr->num_of_channels, sizeof(ddr_channel));
	for (i = 0; i < ddr->num_of_channels; ++i)
	{
		init_ddr_channel(&(ddr->channels[i]), cfg);
	}

	cfg->ddr_cfg.channels.size = ddr->size / ddr->num_of_dimms;
}

ERROR_RET access_to_ddr(ddr *ddr, unsigned int addr, unsigned int size, unsigned int *cost)
{
	
}

void release_ddr(ddr *ddr)
{
	unsigned int i;
	for (i = 0; i < ddr->num_of_channels; ++i)
	{
		release_ddr_channel(&(ddr->channels[i]));
	}
}