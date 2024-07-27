#include "ddr_bank.h"
#include "common.h"

void init_ddr_bank(ddr_bank *bank, ddr_bank_config *cfg, unsigned int banks, unsigned int size, unsigned int page_size, unsigned int bus_width)
{
	bank->bus = bus_width;
	bank->size = size;
	bank->page_size = page_size;
	bank->num_of_rows = cfg->rows;
	bank->num_of_columns = cfg->columns;
	bank->CAS = cfg->CAS;
	bank->RAS = cfg->RAS;
	bank->offset_width = bus_width / banks;
	bank->rows_width = log2int(cfg->rows);
	bank->columns_width = log2int(cfg->columns);
	bank->current_row = cfg->rows;
}

unsigned int access_to_bank(ddr_bank *bank, unsigned int *addr)
{
	unsigned int res, cur_row, cur_column;
	
	cur_column = (*addr >> bank->offset_width) & GET_MASK(bank->columns_width);
	cur_row = (*addr >> (bank->offset_width + bank->columns_width)) & GET_MASK(bank->rows_width);
	
	res = bank->page_size / bank->bus * bank->CAS;
	if(bank->current_row != cur_row)
	{
		res += bank->RAS;
	}
	return res;
}