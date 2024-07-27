#include "ddr_bank.h"
#include "common.h"

void init_ddr_bank(ddr_bank *bank, config *cfg)
{
	bank->bus = cfg->bus_width;
	bank->size = cfg->ddr_cfg.channels.dimms.rank.banks.size;
	bank->page_size = cfg->page_size;
	bank->num_of_rows = cfg->ddr_cfg.channels.dimms.rank.banks.rows;
	bank->num_of_columns = cfg->ddr_cfg.channels.dimms.rank.banks.columns;
	bank->CAS = cfg->ddr_cfg.channels.dimms.rank.banks.CAS;
	bank->RAS = cfg->ddr_cfg.channels.dimms.rank.banks.RAS;
	bank->offset_width = cfg->bus_width / cfg->ddr_cfg.channels.dimms.rank.num_of_banks;
	bank->rows_width = log2int(bank->num_of_rows);
	bank->columns_width = log2int(bank->num_of_columns);
	bank->current_row = bank->num_of_rows;
}

ERROR_RET access_to_bank(ddr_bank *bank, unsigned int addr, unsigned int size, unsigned int *cost)
{
	unsigned int cur_row, cur_column;
	
	cur_column = (addr >> bank->offset_width) & GET_MASK(bank->columns_width);
	cur_row = (addr >> (bank->offset_width + bank->columns_width)) & GET_MASK(bank->rows_width);
	
	*cost = size / bank->bus * bank->CAS;
	if(bank->current_row != cur_row)
	{
		*cost += bank->RAS;
		bank->current_row = cur_row;
	}
	return NO_ERR;
}