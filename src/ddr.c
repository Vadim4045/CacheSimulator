#include "ddr.h"
#include "common.h"

void init_ddr(ddr *ddr, config *cfg)
{
	unsigned int i, j, k, l;
	ddr->cfg = cfg;
	ddr->num_of_banks = cfg->ddr_cfg.num_of_banks;
	ddr->offset_width = log2int(cfg->bus_width);
	ddr->page_size_width = log2int(cfg->page_size);
	ddr->channel_num_width = log2int(cfg->ddr_cfg.num_of_channels);
	ddr->dimm_num_width = log2int(cfg->ddr_cfg.num_of_dimms);
	ddr->rank_num_width = log2int(cfg->ddr_cfg.num_of_ranks);
	ddr->bank_num_width = log2int(ddr->num_of_banks);
	ddr->column_num_width = log2int(cfg->ddr_cfg.row_size / cfg->bus_width);
	ddr->row_num_width = log2int(0x800 * _1K 
					/ cfg->ddr_cfg.num_of_channels 
					/ cfg->ddr_cfg.num_of_dimms 
					/ cfg->ddr_cfg.num_of_ranks 
					/ ddr->num_of_banks 
					/ cfg->ddr_cfg.row_size 
					* _1K);

	ddr->channels = (channel *)calloc(cfg->ddr_cfg.num_of_channels, sizeof(channel));
	for (i = 0; i < cfg->ddr_cfg.num_of_channels; ++i)
	{
		ddr->channels[i].dimms = (dimm *)calloc(cfg->ddr_cfg.num_of_dimms, sizeof(dimm));
		for (j = 0; j < cfg->ddr_cfg.num_of_dimms; ++j)
		{
			ddr->channels[i].dimms[j].ranks = (rank *)calloc(cfg->ddr_cfg.num_of_banks, sizeof(rank));
			for (k = 0; k < cfg->ddr_cfg.num_of_ranks; ++k)
			{
				ddr->channels[i].dimms[j].ranks[k].banks = (unsigned int *)calloc(ddr->num_of_banks, sizeof(unsigned int));
				for (l = 0; l < ddr->num_of_banks; ++l)
				{
					ddr->channels[i].dimms[j].ranks[k].banks[l] = cfg->ddr_cfg.row_size / cfg->bus_width; // first ubnormal
				}
			}
		}
	}
		
}

DDR_RET access_to_ddr(ddr *ddr, unsigned int addr)
{
	unsigned int i, bank_num, row, cur_row;
	unsigned int channel_num, dimm_num, rank_num;
	switch (ddr->cfg->ddr_cfg.channel_pos)
	{
		case 0:
			//	-----------------------------------
			//	| cnannel | dimm | rank | address |
			//	-----------------------------------

			channel_num = (addr >> (ddr->offset_width + ddr->column_num_width + ddr->bank_num_width + ddr->row_num_width 
												+ ddr->rank_num_width + ddr->dimm_num_width)) & GET_MASK(ddr->channel_num_width);

			dimm_num = (addr >> (ddr->offset_width + ddr->column_num_width + ddr->bank_num_width + ddr->row_num_width 
													+ ddr->rank_num_width)) & GET_MASK(ddr->dimm_num_width);

			rank_num = (addr >> (ddr->offset_width + ddr->column_num_width + ddr->bank_num_width + ddr->row_num_width)) 
												& GET_MASK(ddr->rank_num_width);

			row = (addr >> (ddr->offset_width + ddr->column_num_width + ddr->bank_num_width)) & GET_MASK(ddr->row_num_width);
			break;
		case 1:
			//	-----------------------------------
			//	| dimm | cnannel | rank | address |
			//	-----------------------------------

			channel_num = (addr >> (ddr->offset_width + ddr->column_num_width + ddr->bank_num_width + ddr->row_num_width 
											+ ddr->rank_num_width)) & GET_MASK(ddr->channel_num_width);

			dimm_num = (addr >> (ddr->offset_width + ddr->column_num_width + ddr->bank_num_width + ddr->row_num_width 
										+ ddr->channel_num_width + ddr->rank_num_width)) & GET_MASK(ddr->dimm_num_width);

			rank_num = (addr >> (ddr->offset_width + ddr->column_num_width + ddr->bank_num_width + ddr->row_num_width)) 
								& GET_MASK(ddr->rank_num_width);

			row = (addr >> (ddr->offset_width + ddr->column_num_width + ddr->bank_num_width)) & GET_MASK(ddr->row_num_width);
			break;
		case 2:
			//	-----------------------------------
			//	| dimm | rank | cnannel | address |
			//	-----------------------------------

			channel_num = (addr >> (ddr->offset_width + ddr->column_num_width + ddr->bank_num_width 
											+ ddr->row_num_width)) & GET_MASK(ddr->channel_num_width);

			dimm_num = (addr >> (ddr->offset_width + ddr->column_num_width + ddr->bank_num_width + ddr->row_num_width 
												+ ddr->channel_num_width + ddr->rank_num_width)) & GET_MASK(ddr->dimm_num_width);

			rank_num = (addr >> (ddr->offset_width + ddr->column_num_width + ddr->bank_num_width + ddr->row_num_width 
												+ ddr->channel_num_width)) & GET_MASK(ddr->rank_num_width);

			row = (addr >> (ddr->offset_width + ddr->column_num_width + ddr->bank_num_width)) & GET_MASK(ddr->row_num_width);
			break;
		default:
			printf("Wrong Channel position. Will choose 0.");
			exit(EXIT_FAILURE);
	}
	

	if(ddr->cfg->ddr_cfg.IP == 0)
	{
		//	BLOCK_INTERLEAVING
		//	------------------------------------------------
		//	| row_addr | bank_addr |  column_addr | offset |
		//	------------------------------------------------
		bank_num = (addr >> (ddr->offset_width + ddr->column_num_width)) & GET_MASK(ddr->bank_num_width);
		cur_row = ddr->channels[channel_num].dimms[dimm_num].ranks[rank_num].banks[bank_num];
		ddr->channels[channel_num].dimms[dimm_num].ranks[rank_num].banks[bank_num] 
			= (addr >> (ddr->offset_width + ddr->column_num_width)) & GET_MASK(ddr->bank_num_width);
	}
	else
	{
		//	ROW_INTERLEAVING
		//	-----------------------------------------------------------------------
		//	| row_addr | column_addr_high | bank_addr |  column_addr_low | offset |
		//	-----------------------------------------------------------------------
		bank_num = (addr >> (ddr->offset_width + ddr->page_size_width)) & GET_MASK(ddr->bank_num_width);
		cur_row = ddr->channels[channel_num].dimms[dimm_num].ranks[rank_num].banks[bank_num];
		for (i = 0; i < ddr->num_of_banks; ++i)
		{
			ddr->channels[channel_num].dimms[dimm_num].ranks[rank_num].banks[i] = row; // activate all banks
		}
	}

	return (cur_row != row);
}

void release_ddr(ddr *ddr)
{
	unsigned int i, j, k;
	for (i = 0; i < ddr->cfg->ddr_cfg.num_of_channels; ++i)
	{
		for (j = 0; j < ddr->cfg->ddr_cfg.num_of_dimms; ++j)
		{
			for (k = 0; k < ddr->cfg->ddr_cfg.num_of_ranks; ++k)
			{
				free(ddr->channels[i].dimms[j].ranks[k].banks);
			}
			free(ddr->channels[i].dimms[j].ranks);
		}
		free(ddr->channels[i].dimms);
	}
	free(ddr->channels);
}