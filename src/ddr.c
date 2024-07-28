#include "ddr.h"
#include "common.h"

void init_ddr(ddr *ddr, config *cfg)
{
	unsigned int i;
	ddr->cfg = cfg;
	ddr->num_of_banks = cfg->ddr_cfg.num_of_banks;

	ddr->page_size_width = log2int(cfg->page_size);
	ddr->bank_num_width = log2int(ddr->num_of_banks);
	ddr->offset_width = log2int(cfg->bus_width);
	ddr->column_num_width = log2int(cfg->ddr_cfg.row_size / cfg->bus_width);
	ddr->row_num_width = log2int(0x800 * _1K / ddr->num_of_banks / cfg->ddr_cfg.row_size * _1K);

	ddr->banks = (unsigned int *)calloc(ddr->num_of_banks, sizeof(unsigned int));
	for (i = 0; i < ddr->num_of_banks; ++i)
	{
		ddr->banks[i] = cfg->ddr_cfg.row_size / cfg->bus_width; // first ubnormal
	}
}

DDR_RET access_to_ddr(ddr *ddr, unsigned int addr)
{
	unsigned int i, bank, row, cur_row;

	row = (addr >> (ddr->offset_width + ddr->column_num_width + ddr->bank_num_width)) & GET_MASK(ddr->row_num_width);

	if(ddr->cfg->ddr_cfg.IP == 0)
	{
		//	ROW_INTERLEAVING
		//	------------------------------------------------
		//	| row_addr | bank_addr |  column_addr | offset |
		//	------------------------------------------------
		bank = (addr >> (ddr->offset_width + ddr->column_num_width)) & GET_MASK(ddr->bank_num_width);
	}
	else
	{
		//	BLOCK_INTERLEAVING
		//	-----------------------------------------------------------------------
		//	| row_addr | column_addr_high | bank_addr |  column_addr_low | offset |
		//	-----------------------------------------------------------------------
		bank = (addr >> (ddr->offset_width + ddr->page_size_width)) & GET_MASK(ddr->bank_num_width);
		cur_row = ddr->banks[bank];
		for (i = 0; i < ddr->num_of_banks; ++i)
		{
			ddr->banks[i] = row; // activate all banks 
		}
	}

	return (cur_row != row);
}

void release_ddr(ddr *ddr)
{
	free(ddr->banks);
}