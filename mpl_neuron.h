#pragma once
#include <vector>
#include "ActFuncs.h"
class mpl_neuron
{
public:
	mpl_neuron(int id,std::vector<float>* inp,int inps_num, ActFuncs::act_f_type act_f_type);
	~mpl_neuron(void);
	 int id;
     int inputs_number;
	 std::vector<float>*input;
     std::vector<int> neurons_next_layer;
	 int neurons_next_layer_number;
	 std::vector<float> weights;
     float error;
     float output;
     float non_activated_stage;
     float learning_speed;
	 float (*f_activation)(float);
     float(*f_activation_deriv)(float);
	 void init_act_f(ActFuncs::act_f_type);
	 void init_act_f_deriv(ActFuncs::act_f_type);
	 void get_output();
	 void get_output(float *_inp);
	 void get_error(float expected_output);
	 void correct_weights();
	 void correct_weights(float *_input);
	 void correct_weights(int start_inp_id, int end_inp_id);
	 void correct_weights(float *_input,int start_inp_id, int end_inp_id);
	 void enwide_inputs(int new_inputs_num);
	 void reduct_inputs(int inputs_num);
	 void reduct_inputs(int start_input_num,int end_input_num);
	 void connect_next_layer(int next_l_neurons_number);
	 void remove_links_from_next_layer(int start_input_num,int end_input_num);
	 void reset();
private:
	bool is_hidden;
};

