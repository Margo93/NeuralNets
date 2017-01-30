#include "stdafx.h"
#include "ActFuncs.h"
#include <math.h>


ActFuncs::ActFuncs(void)
{
}


ActFuncs::~ActFuncs(void)
{
}

float ActFuncs::f_act_sigmoidal(float input)
{
     return  (float)(1/(1+exp((-1*3)*input)));
}

float ActFuncs::f_act_sigmoidal_deriv(float input)
{ 
	return f_act_sigmoidal(input)*(1-f_act_sigmoidal(input));
} 

float ActFuncs::f_act_thn(float input)
{
	return  (float)((exp(2*input) -1)/(exp(2*input) + 1));
}


float ActFuncs::f_act_thn_deriv(float input)
{ float ch =0;
	 ch=(float)((exp(input) + exp((-1)*input))/2);
	 return 1/(pow(ch,2));
}

float ActFuncs::f_act_linear(float input)
{
	float linear_k_param = 1;
	return  linear_k_param*input;
}

float ActFuncs::f_act_linear_deriv(float input)
{
	float linear_k_param = 1;
	return linear_k_param;
}

