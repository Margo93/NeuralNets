#pragma once
#include <vector>
class LearningPair
{
public:
	LearningPair(int inp_l,int outp_l,float* inp,float *expected_outp);
	~LearningPair(void);
	float* input;
	float*expected_output;
private:
	int input_lenght;
	int output_lenght;
};

