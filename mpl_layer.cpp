#include "stdafx.h"
#include "mpl_layer.h"
#include "MatrixOperations.h"
#include <iostream>
#include <iomanip>


mpl_layer::mpl_layer(void)
{
}

mpl_layer::mpl_layer(int _neurons_number)
{
	    neurons_number = _neurons_number;
		neurons.reserve(neurons_number);
		outputs.reserve(neurons_number);
		no_dropout_mask.reserve(neurons_number);
		for(int i=0;i<_neurons_number;i++)
		{
			outputs.push_back(0);
			no_dropout_mask.push_back(true);
		}
		mpl_next_ptr=NULL;
		mpl_prev_ptr=NULL;
}

//for layers already which have neurons


void mpl_layer::add_neurons(layer_description new_neurs_descr)
{  
    outputs.resize(neurons_number+new_neurs_descr.neurons_num,0);
	no_dropout_mask.resize(neurons_number+new_neurs_descr.neurons_num,true);
	mpl_neuron* cur_neur = &(neurons[neurons_number-1]);
	neurons.resize(neurons_number+new_neurs_descr.neurons_num,(mpl_neuron(cur_neur->id+1,cur_neur->input,cur_neur->inputs_number,new_neurs_descr.act_f)));
	neurons_number +=new_neurs_descr.neurons_num;

	int counter =0;
	for(int j=neurons_number-new_neurs_descr.neurons_num;j<neurons_number;j++)
	{neurons[j].id+=counter;   
	neurons[j].input = neurons[j-1].input;
	counter++;
	}
	

	//last_neur = &(neurons[neurons_number-1]);
	if(mpl_next_ptr!=NULL)
	{
		int next_l_neur_num = mpl_next_ptr->neurons_number;

		for(int j=neurons_number-new_neurs_descr.neurons_num-1;j<neurons_number;j++)
			neurons[j].connect_next_layer(next_l_neur_num);

		mpl_next_ptr->enwide_inputs(new_neurs_descr.neurons_num);
	}

	
	if(mpl_prev_ptr!=NULL)
		mpl_prev_ptr->connect_next_layer(mpl_prev_ptr->mpl_next_ptr);

}

void mpl_layer::remove_neurons(int start_neuron_id,int end_neuron_id)
{
	int removing_neurs_num = end_neuron_id-start_neuron_id+1;
	for(int i=start_neuron_id;i<end_neuron_id;i++)
	{
		outputs.erase(outputs.end()-1);
		no_dropout_mask.erase(no_dropout_mask.end()-1);
		neurons.erase(neurons.end()-1);
	}
	neurons_number-=removing_neurs_num;
	outputs.shrink_to_fit();
	no_dropout_mask.shrink_to_fit();
	neurons.shrink_to_fit();

	for(int j=start_neuron_id;j<neurons_number;j++)
	neurons[j].id-=removing_neurs_num;

	//renew links
	if(mpl_next_ptr!=NULL)
		mpl_next_ptr->reduct_inputs(start_neuron_id,end_neuron_id);

	if(mpl_prev_ptr!=NULL)
		mpl_prev_ptr->remove_links_from_next_layer(start_neuron_id,end_neuron_id);
	
}

mpl_layer::~mpl_layer(void)
{
	neurons.clear();
	outputs.clear();
//	std::cout<<std::endl<<"LAYER REMOVED"<<std::endl;
}

void mpl_layer::connect_next_layer(mpl_layer* _next_layer)
 {		
	 mpl_next_ptr=_next_layer;
	 int next_layer_neur_num = _next_layer->neurons_number;
	 for(int j=0;j< neurons_number;j++)
	 {
		neurons[j].neurons_next_layer.clear();
		neurons[j].neurons_next_layer.reserve(next_layer_neur_num);
		neurons[j].neurons_next_layer_number = next_layer_neur_num;

           for (int i = 0; i < next_layer_neur_num; i++)
			neurons[j].neurons_next_layer.push_back(i);   
	 }
}



void mpl_layer::renew_prev_layer_references(mpl_layer* _prev_layer)
{
	mpl_prev_ptr = _prev_layer;
	int prev_layer_neur_num = _prev_layer->neurons_number;

	 for(int j=0;j< neurons_number;j++)
	 {
           for (int i = 0; i < prev_layer_neur_num; i++)
				   neurons[j].input = &(_prev_layer->outputs);      
	 }
}

void mpl_layer::renew_next_layer_references(mpl_layer *_next_layer)
{
	mpl_next_ptr = _next_layer;
}
		//for hidden layers
void mpl_layer::get_outputs()
{ 
	 for(int j=0;j< neurons_number;j++)
		{
			if(no_dropout_mask[j])
			{neurons[j].get_output();
			outputs[j] = neurons[j].output;}
			else
				outputs[j]=0;
		}
}

void mpl_layer::get_outputs(float* _input)
{ 
	
   for (int j = 0; j < neurons_number; j++)
      { if(no_dropout_mask[j])
		 { neurons[j].get_output(_input);
         outputs[j] =neurons[j].output;}
   		else
		outputs[j]=0;
      }
}


void mpl_layer::get_errors()
{
//W transp*sigma_next_layer*f_derived(ul)
	float err=0;
	mpl_neuron *cur_neur;
	mpl_neuron *next_l_neur;
	int next_l_neur_id=0;
	for(int j=0;j< neurons_number;j++)
	{cur_neur = &(neurons[j]);
	for (int q = 0; q < cur_neur->neurons_next_layer_number; q++)
	{ next_l_neur_id=cur_neur->neurons_next_layer[q];
	if(mpl_next_ptr->no_dropout_mask[next_l_neur_id])
		{next_l_neur = &(mpl_next_ptr->neurons[next_l_neur_id]);
			err+=next_l_neur->error*next_l_neur->weights[cur_neur->id];
	    }
	}
		cur_neur->error=err;
	}
}

void mpl_layer::get_errors(float* _expected_outp)
{ for(int j=0;j< neurons_number;j++)
neurons[j].get_error(_expected_outp[j]);
}

void mpl_layer::correct_weights()
{ for(int j=0;j< neurons_number;j++)
{if(no_dropout_mask[j])
neurons[j].correct_weights();}
}

void mpl_layer::correct_weights(float *_input)
{ for(int j=0;j< neurons_number;j++)
{if(no_dropout_mask[j])
neurons[j].correct_weights(_input);}
}

void mpl_layer::correct_weights_partly(int start_inp_id,int end_inp_id)
{ for(int j=0;j< neurons_number;j++)
{if(no_dropout_mask[j])
neurons[j].correct_weights(start_inp_id,end_inp_id);}
}

void mpl_layer::correct_weights_partly(float *_input,int start_inp_id,int end_inp_id)
{ for(int j=0;j< neurons_number;j++)
{if(no_dropout_mask[j])
neurons[j].correct_weights(_input,start_inp_id,end_inp_id);}
}

void mpl_layer::correct_weights(int start_neur_position,int end_neur_position)
{ if(end_neur_position>neurons_number)
    end_neur_position=neurons_number;

	for(int j=start_neur_position;j<=end_neur_position;j++)
	{
		if(no_dropout_mask[j])
		neurons[j].correct_weights();

		if(mpl_next_ptr!=NULL)
			mpl_next_ptr->correct_weights_partly(start_neur_position,end_neur_position);
	}
}

void mpl_layer::correct_weights(float *_input,int start_neur_position,int end_neur_position)
{ if(end_neur_position>neurons_number)
    end_neur_position=neurons_number;

	for(int j=start_neur_position;j<=end_neur_position;j++)
	{
		if(no_dropout_mask[j])
		neurons[j].correct_weights(_input);

		if(mpl_next_ptr!=NULL)
			mpl_next_ptr->correct_weights_partly(start_neur_position,end_neur_position);
	}
}

void mpl_layer::resize_inputs(int old_inputs_num,int new_inputs_num)
{
	if(old_inputs_num>new_inputs_num)
		reduct_inputs(old_inputs_num-new_inputs_num);

	if(old_inputs_num<new_inputs_num)
		enwide_inputs(new_inputs_num-old_inputs_num);
}

void mpl_layer::enwide_inputs(int new_inps_number)
{
	for(int i=0;i<neurons_number;i++)
		neurons[i].enwide_inputs(new_inps_number);
}

void mpl_layer::reduct_inputs(int inputs_num)
{
	for(int i=0;i<neurons_number;i++)
		neurons[i].reduct_inputs(inputs_num);
}

void mpl_layer::reduct_inputs(int start_inp_num,int end_inp_num)
{
	for(int i=0;i<neurons_number;i++)
		neurons[i].reduct_inputs(start_inp_num,end_inp_num);
}

void mpl_layer::remove_links_from_next_layer(int start_input_num,int end_input_num)
{
	for(int i=0;i<neurons_number;i++)
		neurons[i].remove_links_from_next_layer(start_input_num,end_input_num);
}

void mpl_layer::reset()
{
	for(int i=0; i<neurons_number;i++)
	{
		neurons[i].reset();
		outputs[i]=0;
	}
};

//
void mpl_layer::set_dropout(float P)
{
	 srand(time(0));
	 int neur_num_for_dropout =(int)(P*float(neurons_number));
	 int id_for_dropout;
	 for(int i=0; i<neur_num_for_dropout;i++)
	 {
		 id_for_dropout=(rand()%neurons_number);
		 no_dropout_mask[id_for_dropout]=false;
	 }
}

void mpl_layer::reset_dropout()
{
	for(int i=0;i<neurons_number;i++)
	{
		no_dropout_mask[i]=true;
	}
}

void mpl_layer::print_output()
{	std::cout<<std::endl;
	for(int i=0;i<neurons_number;i++)
		std::cout<<std::setw(6)<<std::setprecision(4)<<std::fixed<<outputs[i]<<"  ";
	std::cout<<std::endl;
}

void mpl_layer::print_input()
{	std::cout<<std::endl;
for(int j=0;j<neurons_number;j++)
{for(int i=0;i<neurons[j].inputs_number;i++)
std::cout<<std::setw(6)<<std::setprecision(4)<<std::fixed<<(*neurons[j].input)[i]<<"  ";
     }
	std::cout<<std::endl<<std::endl;
}

void mpl_layer::print_error()
{	std::cout<<std::endl;
	for(int i=0;i<neurons_number;i++)
		std::cout<<std::setw(6)<<std::setprecision(4)<<std::fixed<<neurons[i].error<<"  ";
	std::cout<<std::endl;
}