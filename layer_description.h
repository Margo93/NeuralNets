#pragma once
#include "ActFuncs.h"
class layer_description
{
public:
	layer_description(int _neurons_num,ActFuncs::act_f_type act_f);
	~layer_description(void);
	ActFuncs::act_f_type act_f;
    int neurons_num;
};

