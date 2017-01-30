#include "stdafx.h"
#include "mpl_neuron.h"
#include "MatrixOperations.h"
#include <iostream>

mpl_neuron::mpl_neuron(int _id,std::vector<float> *_inp,int _inps_num, ActFuncs::act_f_type _act_f_type)
{
	id=_id;
	input = _inp;
	init_act_f(_act_f_type);
	init_act_f_deriv(_act_f_type);
	inputs_number = _inps_num;
	MatrixOperations::init_vector_random(&(weights),inputs_number);
	learning_speed=0.7f;
	neurons_next_layer_number=0;
}

mpl_neuron::~mpl_neuron(void)
{
	weights.clear();
	neurons_next_layer.clear();
}

void mpl_neuron::get_output()
{ non_activated_stage = 0;
     for (int i = 0; i < inputs_number; i++)
                    non_activated_stage += (*input)[i] * weights[i];
  output = f_activation(non_activated_stage);
}

void mpl_neuron::get_output(float* _inp)
{ 
	non_activated_stage = 0;
     for (int i = 0; i < inputs_number; i++)
		 non_activated_stage += _inp[i] * weights[i];
  output = f_activation(non_activated_stage);
}

//for output layer
void mpl_neuron::get_error(float expected_output)
{
	error = (expected_output - output) * f_activation_deriv(non_activated_stage);
}

void mpl_neuron::correct_weights()
{
	 for (int i = 0; i < inputs_number; i++)
         weights[i] += error * (*input)[i] * learning_speed;
}

void mpl_neuron::correct_weights(float *_input)
{
	 for (int i = 0; i < inputs_number; i++)
		 weights[i] += error * _input[i] * learning_speed;
}

void mpl_neuron::correct_weights(int start_inp_id, int end_inp_id)
{ 
	for (int i = start_inp_id; i <= end_inp_id; i++)
         weights[i] += error * (*input)[i] * learning_speed;
}

void mpl_neuron::correct_weights(float *_input,int start_inp_id, int end_inp_id)
{
	for (int i = start_inp_id; i <= end_inp_id; i++)
		 weights[i] += error * _input[i] * learning_speed;
}

void mpl_neuron::enwide_inputs(int new_inputs_num)
{
	weights.resize(inputs_number+new_inputs_num,0.5f);
	inputs_number+=new_inputs_num;
}

void mpl_neuron::reduct_inputs(int inputs_num)
{
	for(int i=0;i<inputs_num;i++)
	weights.erase(weights.end()-1);

	weights.shrink_to_fit();
	inputs_number-=inputs_num;
}

void mpl_neuron::reduct_inputs(int start_input_num,int end_input_num)
{
	int inps_for_delete= end_input_num-start_input_num+1;
	for(int i=0;i<inps_for_delete;i++)
		weights.erase(weights.begin()+start_input_num);

	weights.shrink_to_fit();
	inputs_number-=inps_for_delete;
}


void mpl_neuron::connect_next_layer(int next_layer_neurons_number)
{
	neurons_next_layer_number=next_layer_neurons_number;
    neurons_next_layer.reserve(next_layer_neurons_number);
	for(int i=0;i<next_layer_neurons_number;i++)
			neurons_next_layer.push_back(i);
}

void mpl_neuron::remove_links_from_next_layer(int start_input_num,int end_input_num)
{
	for(int i=0;i<(end_input_num-start_input_num+1);i++)
		neurons_next_layer.erase(neurons_next_layer.end()-1);

	neurons_next_layer.shrink_to_fit();
	neurons_next_layer_number = neurons_next_layer.size();
}

void mpl_neuron::reset()
{
	MatrixOperations::init_vector_random(&(weights),inputs_number);

	error=0;
	output=0;
	non_activated_stage=0;
}

void mpl_neuron::init_act_f( ActFuncs::act_f_type f_type)
{
	switch (f_type)
	{
	case ActFuncs::SIGM:
		f_activation = *(ActFuncs::f_act_sigmoidal);
		break;
	case ActFuncs::THN:
		f_activation = *(ActFuncs::f_act_thn);
		break;
	case ActFuncs::LINEAR:
		f_activation = *(ActFuncs::f_act_linear);
		break;
	default:
		f_activation = *(ActFuncs::f_act_sigmoidal);
		break;
	}
}

	
void mpl_neuron::init_act_f_deriv( ActFuncs::act_f_type f_type)
{
	switch (f_type)
	{
	case ActFuncs::SIGM:
		f_activation_deriv = *(ActFuncs::f_act_sigmoidal_deriv);
		break;
	case ActFuncs::THN:
		f_activation_deriv = *(ActFuncs::f_act_thn_deriv);
		break;
	case ActFuncs::LINEAR:
		f_activation_deriv = *(ActFuncs::f_act_linear_deriv);
		break;
	default:
		f_activation_deriv = *(ActFuncs::f_act_sigmoidal_deriv);
		break;
	}
}