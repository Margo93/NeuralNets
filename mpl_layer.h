#pragma once
#include <vector>
#include "mpl_neuron.h"
#include "layer_description.h"
class mpl_layer
{
public:
	mpl_layer(int _neurons_number);
	mpl_layer(void);
	~mpl_layer(void);
	int neurons_number;
    std::vector<mpl_neuron> neurons;
	std::vector<bool> no_dropout_mask;
	std::vector<float> outputs;
	mpl_layer *mpl_prev_ptr;
	mpl_layer *mpl_next_ptr;
	void connect_next_layer(mpl_layer* _mpl_next_layer);
	void mpl_layer::renew_next_layer_references(mpl_layer* _prev_layer);
	void mpl_layer::renew_prev_layer_references(mpl_layer* _prev_layer);
	void add_neurons(layer_description new_neurs_descr);
	void remove_neurons(int start_neuron_id,int end_neuron_id);
	void resize_inputs(int old_inputs_num,int new_inputs_num);
	virtual void get_outputs();
	void get_outputs(float* _input);
	void get_errors();
	void get_errors(float* expected_output);
	void correct_weights(float* _input);	
	void correct_weights();	
	void correct_weights(int start_neur_position, int end_neur_position);
	void correct_weights(float *_input,int start_neur_position,int end_neur_position);
	void correct_weights_partly(int start_inp_id, int end_inp_id);
	void correct_weights_partly(float *_input,int start_inp_id, int end_inp_id);
	void reset();
	void set_dropout(float probability);
	void reset_dropout();
	void print_output();
	void print_input();
	void print_error();
	void reduct_inputs(int start_inp_num,int end_inp_num);
	void remove_links_from_next_layer(int start_input_num,int end_input_num);
private:
	void enwide_inputs(int new_inputs_num);
	void reduct_inputs(int inputs_num);
};

