#pragma once
//activation function "presses" its input and produces output for neuron
class ActFuncs
{
public:
	ActFuncs(void);
	~ActFuncs(void);
	static float f_act_sigmoidal(float);
	static float f_act_sigmoidal_deriv(float);
	static float f_act_thn(float);
	static float f_act_thn_deriv(float);
	static float f_act_linear(float);
	static float f_act_linear_deriv(float);
	enum act_f_type {SIGM,THN,LINEAR};
};

