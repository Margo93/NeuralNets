#include "stdafx.h"
#include "layer_description.h"


layer_description::layer_description(int _neurons_number, ActFuncs::act_f_type _act_func)
{act_f = _act_func;
 neurons_num = _neurons_number;
}


layer_description::~layer_description(void)
{
}
