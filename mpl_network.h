#pragma once
#include <vector>
#include "mpl_layer.h"
#include "LearningPair.h"
class mpl_network
{
 public:
	  mpl_network(int inputs_count,layer_description first_layer_description);
	 ~mpl_network(void);
        int iterations_number;
        int result_vector_lenght;
        int layers_num;
        int total_neurons_number;
		int input_len;
        float EMS_precision;
		float rounded_precision;
        float time_sec;
		void recognize(float* input);
		void learn(std::vector<LearningPair> learning_pairs, float precision,float safe_counter,int batch_size);
		void learn(std::vector<LearningPair> learning_pairs, float precision,float safe_counter,float dropout_probability,int batch_size);
		void partly_learn(std::vector<LearningPair> learning_pairs,float epsilum, float safe_counter,int layer,int start_neur_position, int end_neur_position);
		void partly_learn(std::vector<LearningPair> learning_pairs,float epsilum, float safe_counter,int start_layer_pos,int end_layer_pos);
		void send_signal_front(float* input);
		void send_signal_back(float* expected_output);
		void correct_weights(float* input);
		void correct_weights(float *_input,int layer_num, int start_neur_pos,int end_neur_pos);
		void correct_weights(float *_input,int layer_num);
		void print_info();
		void reset();
		void testing(std::vector<LearningPair> learning_pairs);
		std::vector<mpl_layer> _layers;
		void insert_layer(int layer_position,layer_description _layer_descr);
		void push_back_layer(layer_description _layer_descr);
		void remove_layer(int layer_position);
		void add_neurons(int layer_num, layer_description new_neurons);
		void remove_neurons(int layer_num,int start_pos, int end_pos);
		//void remove_layer(int layer_pos);
		mpl_layer *output;
private:
	bool is_layer_in_range(int layer_id);
	void make_iteration(LearningPair* lp);
	void make_iteration(LearningPair* lp,int start_layer_num,int end_layer_num);
	void make_iteration(LearningPair* lp,int layer_num,int start_neuron_num,int end_neuron_num);
	float count_energy_for_iteration(LearningPair* lp);
	void set_dropout(float P);
	void reset_dropout();
};

