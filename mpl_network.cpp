#include "stdafx.h"
#include "mpl_network.h"
#include <math.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include "MatrixOperations.h"

mpl_network::mpl_network(int _inputs_number,layer_description first_layer_description)
{
iterations_number =0;
EMS_precision = 0;
rounded_precision=0;
time_sec=0;
layers_num =1;
input_len = _inputs_number;
result_vector_lenght = first_layer_description.neurons_num;
total_neurons_number = first_layer_description.neurons_num;
_layers.push_back(mpl_layer(first_layer_description.neurons_num));

for (int i = 0; i < first_layer_description.neurons_num; i++)
	{
		mpl_neuron mpl= mpl_neuron(i, NULL,_inputs_number,first_layer_description.act_f); 
		_layers[0].neurons.push_back(mpl); 
	}
 output = &(_layers[0]);
}

void mpl_network::push_back_layer(layer_description _layer_descr)
{
	int ln= layers_num;
	insert_layer(ln,_layer_descr);
    result_vector_lenght = _layer_descr.neurons_num;
}

void mpl_network::insert_layer(int layer_pos,layer_description _layer_descr)
{
	
	if(layer_pos>0 && layer_pos<=layers_num)
	{
		int hl=  layer_pos;
		std::vector<mpl_layer>::iterator layer_it = _layers.begin()+layer_pos;
		_layers.insert(layer_it,mpl_layer(_layer_descr.neurons_num));

		for (int j = 0; j <_layer_descr.neurons_num; j++)
		_layers[hl].neurons.push_back(mpl_neuron(j,&(_layers[hl-1].outputs),_layers[hl-1].neurons_number,_layer_descr.act_f));
		//connection
		_layers[hl-1].connect_next_layer(&(_layers[hl]));
		if(hl<layers_num)
		{	_layers[hl].connect_next_layer(&(_layers[hl+1]));
			_layers[hl+1].resize_inputs(_layers[hl-1].neurons_number,_layer_descr.neurons_num);
		}
		//renew old references
		for(int l=1; l<_layers.size();l++)
		{
		_layers[l-1].renew_next_layer_references(&(_layers[l]));
		_layers[l].renew_prev_layer_references(&(_layers[l-1]));
		}
		layers_num ++;
		output = &(_layers[layers_num-1]);
		total_neurons_number+=_layer_descr.neurons_num;
	}
	else
		std::cout<<std::endl<<"layer's position is out of range"<<std::endl;
}

void mpl_network::remove_layer(int layer_pos)
{
	if(is_layer_in_range(layer_pos))
	{
		int hl=  layer_pos;
		int removed_l_neurons_number=_layers[layer_pos].neurons_number;
		bool is_hidden_removed = true;
		if(layer_pos==layers_num-1 && layer_pos==0)
			is_hidden_removed = false;

		total_neurons_number-=removed_l_neurons_number;
		layers_num --;
		std::vector<mpl_layer>::iterator layer_it = _layers.begin()+layer_pos;
		_layers.erase(layer_it);
		_layers.shrink_to_fit();

		//reconnection
		if(is_hidden_removed)
		{
			_layers[hl-1].connect_next_layer(&(_layers[hl]));
			_layers[hl].resize_inputs(removed_l_neurons_number,_layers[hl-1].neurons_number);
		}

		if(layer_pos==0)
			_layers[hl].resize_inputs(removed_l_neurons_number,input_len);
		
		//renew old references
		for(int l=1; l<_layers.size();l++)
		{
		_layers[l-1].renew_next_layer_references(&(_layers[l]));
		_layers[l].renew_prev_layer_references(&(_layers[l-1]));
		}

		output = &(_layers[layers_num-1]);
	}
}

void mpl_network::add_neurons(int layer_num, layer_description new_neurons)
{
	if(is_layer_in_range(layer_num))
	{
		_layers[layer_num].add_neurons(new_neurons);
		total_neurons_number+=new_neurons.neurons_num;
	}
}

void mpl_network::remove_neurons(int layer_num,int start_pos, int end_pos)
{
	if(is_layer_in_range(layer_num))
	{	int neurons_to_remove = end_pos-start_pos+1;
		if(	_layers[layer_num].neurons_number>=neurons_to_remove)
			{
				_layers[layer_num].remove_neurons(start_pos,end_pos);	
				total_neurons_number -=(end_pos-start_pos+1);
			}
	else
		std::cout<<std::endl<<"neurons number out of range"<<std::endl;
	}
}

void mpl_network::recognize(float* _input)
 {
	send_signal_front(_input);
	output->print_output();
 }

void mpl_network::learn(std::vector<LearningPair> learning_pairs, float eps, float safe_counter,int batch_size)
        {
          iterations_number = 0;
		  int lps_size = learning_pairs.size();
		  float E=1;
		  float avg_E=0;
		  int lp=0;
		  LearningPair * cur_lp_ptr;
		  //Timer
		  srand(time(0));
          auto start = std::chrono::high_resolution_clock::now();
		  auto stop = start;

            while (iterations_number < safe_counter && E> eps)
            { avg_E=0;

				for(int i=0; i<batch_size;i++)
				{
				int lp=rand()%lps_size;
				cur_lp_ptr = &(learning_pairs[lp]);
				make_iteration(cur_lp_ptr);
				E = count_energy_for_iteration(cur_lp_ptr);
				avg_E+=E;
				}
				avg_E = avg_E/batch_size;
            }
			 stop = std::chrono::high_resolution_clock::now();
			 time_sec =((float)(std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count())/1000);
             EMS_precision = (1-avg_E)*100;
}

void mpl_network::learn(std::vector<LearningPair> learning_pairs, float eps, float safe_counter,float dropout_probability,int batch_size)
        {
          iterations_number = 0;
		  int lps_size = learning_pairs.size();
		  float E=1;
		  float avg_E=0;
		  int lp=0;
		  LearningPair * cur_lp_ptr;
		  //Timer
		  srand(time(0));
          auto start = std::chrono::high_resolution_clock::now();
		  auto stop = start;

            while (iterations_number < safe_counter && E> eps)
			{ avg_E=0;
				set_dropout(dropout_probability);
				for(int i=0; i<batch_size;i++)
				{
				lp=rand()%lps_size;
				cur_lp_ptr = &(learning_pairs[lp]);
				make_iteration(cur_lp_ptr);
			    E = count_energy_for_iteration(cur_lp_ptr);
				avg_E+=E;
				}
				reset_dropout();
				avg_E = avg_E/batch_size;
            }
			 stop = std::chrono::high_resolution_clock::now();
			 time_sec =((float)(std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count())/1000);
			 EMS_precision = std::abs(1-avg_E)*100;
}

void mpl_network::partly_learn(std::vector<LearningPair> learning_pairs,float eps, float safe_counter,int layer,int start_neuron_pos,int end_neuron_pos)
        {
          iterations_number = 0;
		  int lps_size = learning_pairs.size();
		  float E=1;
		  float avg_E=0;
		  int lp=0;
		  LearningPair * cur_lp_ptr;
		  //Timer
		  srand(time(0));
          auto start = std::chrono::high_resolution_clock::now();
		  auto stop = start;

            while (iterations_number < safe_counter && E> eps)
            {

				int lp=rand()%lps_size;
				cur_lp_ptr = &(learning_pairs[lp]);
				make_iteration(cur_lp_ptr,layer,start_neuron_pos,end_neuron_pos);
				E = count_energy_for_iteration(cur_lp_ptr);
				avg_E+=E;
            }
			 stop = std::chrono::high_resolution_clock::now();
			 time_sec =((float)(std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count())/1000);
             EMS_precision = (1-avg_E)*100;
}

void mpl_network::partly_learn(std::vector<LearningPair> learning_pairs,float eps, float safe_counter,int start_layer_pos,int end_layer_pos)
   {
          iterations_number = 0;
		  int lps_size = learning_pairs.size();
		  float E=1;
		  float avg_E=0;
		  int lp=0;
		  LearningPair * cur_lp_ptr;
		  //Timer
		  srand(time(0));
          auto start = std::chrono::high_resolution_clock::now();
		  auto stop = start;

            while (iterations_number < safe_counter && E> eps)
            {
				int lp=rand()%lps_size;
				cur_lp_ptr = &(learning_pairs[lp]);
				make_iteration(cur_lp_ptr,start_layer_pos,end_layer_pos);
				E = count_energy_for_iteration(cur_lp_ptr);
				avg_E+=E;
            }
			 stop = std::chrono::high_resolution_clock::now();
			 time_sec =((float)(std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count())/1000);
             EMS_precision = (1-avg_E)*100;
}

void mpl_network::make_iteration(LearningPair* lp)
{
	iterations_number++;
    send_signal_front(lp->input);
    send_signal_back(lp->expected_output);
    correct_weights(lp->input);
}

void mpl_network::make_iteration(LearningPair* lp,int start_layer_num,int end_layer_num)
{
	iterations_number++;
    send_signal_front(lp->input);
    send_signal_back(lp->expected_output);
	for(int ln=start_layer_num;ln<=end_layer_num;ln++)
    correct_weights(lp->input,ln);
}

void mpl_network::make_iteration(LearningPair* lp,int layer_num,int start_neuron_pos,int end_neuron_pos)
{
	iterations_number++;
    send_signal_front(lp->input);
    send_signal_back(lp->expected_output);
    correct_weights(lp->input,layer_num,start_neuron_pos,end_neuron_pos);
}

float  mpl_network::count_energy_for_iteration(LearningPair* lp)
{
	float E=0;
    for (int i = 0; i < result_vector_lenght; i++)
	E+= (float)(0.5*(std::pow((lp->expected_output[i] - output->neurons[i].output),2)));
	return E/result_vector_lenght;
}

void mpl_network::set_dropout(float P)
{
	for(int i=0; i<layers_num;i++)
	_layers[i].set_dropout(P);

}
void mpl_network::reset_dropout()
{
	for(int i=0; i<layers_num;i++)
		_layers[i].reset_dropout();
}

void mpl_network::send_signal_front(float *_input)
        {
			_layers[0].get_outputs(_input);
		//	MatrixOperations::print_vector(&(_layers[0].outputs),_layers[0].neurons_number);
			for (int i = 1; i < layers_num; i++)
			{//	_layers[i].print_input();
				_layers[i].get_outputs();
			//	_layers[i].print_output();
			}


        }

void mpl_network::send_signal_back(float* expected_output)
	{   
		output->get_errors(expected_output);
		for (int i = layers_num-2; i >= 0; i--)
			_layers[i].get_errors();
	}

void mpl_network::correct_weights(float *_input)
{
	_layers[0].correct_weights(_input);
	for (int i = 1; i < layers_num; i++)
		_layers[i].correct_weights();
}

void mpl_network::correct_weights(float*_input, int layer_pos)
{
	if(layer_pos==0)
	_layers[0].correct_weights(_input);
	else
	_layers[layer_pos].correct_weights();	
}

void mpl_network::correct_weights(float *_input,int layer_num, int start_neur_pos,int end_neur_pos)
{ 
	if(layer_num==0)
	_layers[layer_num].correct_weights(_input,start_neur_pos,end_neur_pos);
	else
	_layers[layer_num].correct_weights(start_neur_pos,end_neur_pos);
}

void mpl_network::reset()
{
		for (int i = 0; i < layers_num; i++)
			_layers[i].reset();

		EMS_precision=0;
		iterations_number=0;
		time_sec=0;
}

void mpl_network::testing(std::vector<LearningPair> learning_pairs)
{
	 int testing_iterations_number = learning_pairs.size();
	 float E=0;
	 srand(time(0));
	 int lp=0;
	 LearningPair* cur_lp;
	 float errors_number=0;

	 for(int i=0; i<testing_iterations_number; i++)
	 {
		 lp=rand()%learning_pairs.size();
		 cur_lp = &(learning_pairs[lp]);
		 send_signal_front(cur_lp->input);
		 errors_number+=MatrixOperations::is_not_match(&(output->outputs),cur_lp->expected_output,result_vector_lenght);
		 E+=count_energy_for_iteration(cur_lp);                   
	 }
	
	rounded_precision = ((testing_iterations_number-errors_number)/testing_iterations_number)*100;
	E = E / (result_vector_lenght*testing_iterations_number);
	EMS_precision= (1-E)*100;
}

bool mpl_network::is_layer_in_range(int layer_num)
{
	if(layer_num>=0 && layer_num<layers_num)
		return true;
	else
	{
		std::cout<<std::endl<<"layer's id is out of range"<<std::endl;
		return false;
	}

}
void mpl_network::print_info()

{
	std::cout<<std::endl<<"EMS precision: "<<EMS_precision<<" %";
	std::cout<<std::endl<<"rounded precision: "<<rounded_precision<<" %";
	std::cout<<std::endl<<"total iterations number : "<<iterations_number;
	std::cout<<std::endl<<"total learning time: "<<time_sec<<" seconds";
	std::cout<<std::endl<<layers_num<<" layers "<<" ("<<total_neurons_number<<" neurons)"<<std::endl;
}


mpl_network::~mpl_network(void)
{
	_layers.clear();
}
