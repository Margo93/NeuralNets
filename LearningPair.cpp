#include "stdafx.h"
#include "LearningPair.h"


LearningPair::LearningPair(int inp_l,int outp_l,float* inp,float *expected_outp)
{

	input_lenght = inp_l;
	output_lenght = outp_l;
	input = inp;
	expected_output = expected_outp;
}


LearningPair::~LearningPair(void)
{
}
