// Falman_netw_cpp.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "mpl_layer.h"
#include "MatrixOperations.h"
#include "LearningPair.h"
#include <vector>
#include "mpl_network.h"
#include "inputs_loader.h"
#include <iostream>
#include <chrono>

int _tmain(int argc, _TCHAR* argv[])
{	//load inputs from files
	std::vector<std::vector<float>> possible_inputs = inputs_loader::get_all_input_vectors_from_files();
	//create learning pairs
	int inputs_num = possible_inputs.size();
	int min_result_vector_lenght= inputs_loader::get_min_result_vector_lenght(inputs_num);
	//changable, must not be< min_result_vector_lenght
	int result_vector_lenght= min_result_vector_lenght;
	//expected outputs
	std::vector<float*> all_targets = inputs_loader::get_auto_outputs(inputs_num,result_vector_lenght);
	int input_lenght = inputs_loader::get_min_inp_len(&(possible_inputs));
	//create learning pairs
	int learning_samples_num=possible_inputs.size();
	int testing_samples_num= 100;
	std::vector<float*> learning_input_vectors = inputs_loader::create_input_samples(possible_inputs,learning_samples_num,0);
	std::vector<float*> testing_input_vectors = inputs_loader::create_input_samples(possible_inputs,testing_samples_num,0.01f);

	std::vector<LearningPair> learning_lps;
	std::vector<LearningPair> testing_lps;

	//creation of learning samples and testing samples
	learning_lps.reserve(learning_samples_num);
	testing_lps.reserve(testing_samples_num);
	int inp=0;
	for(int i=0; i<learning_samples_num;i++)
	{
		inp = i%inputs_num;
		learning_lps.push_back(LearningPair(input_lenght,result_vector_lenght,learning_input_vectors[i],all_targets[inp]));
	}
		for(int i=0; i<testing_samples_num;i++)
	{
		inp = i%inputs_num;
		testing_lps.push_back(LearningPair(input_lenght,result_vector_lenght,testing_input_vectors[i],all_targets[inp]));
	}

//simple network creation

/*		mpl_network test_netw_1 = mpl_network(input_lenght,layer_description(20,ActFuncs::SIGM));
//last layer's neurons number = outputs number
		test_netw_1.push_back_layer(layer_description(result_vector_lenght,ActFuncs::SIGM));
/*
//learn
	float max_iters_num = 10000;
//max error
	float max_epsilum=0.001f;

	test_netw_1.learn(learning_lps,max_epsilum,max_iters_num,learning_lps.size());
	test_netw_1.testing(testing_lps);
    test_netw_1.print_info();

/*	test_netw_1.insert_layer(1,layer_description(7,ActFuncs::SIGM));
	test_netw_1.partly_learn(learning_lps,max_epsilum,max_iters_num,1,test_netw_1.layers_num-1);

	test_netw_1.testing(testing_lps);
    test_netw_1.print_info();

	test_netw_1.remove_layer(1);
	test_netw_1.partly_learn(learning_lps,max_epsilum,max_iters_num,test_netw_1.layers_num-1,test_netw_1.layers_num-1);

	test_netw_1.testing(testing_lps);
    test_netw_1.print_info();

*/

/*		//DROPOUT TESTING
	float dropout_p=0.1f;
	float locals_perc =0;
	float experiments_number =10;
	float avg_time =0;
	//learn
	float max_iters_num = 10000;
	int batch_size = learning_lps.size();
	//max error
	float max_epsilum=0.00001f;

			for(int j=0;j<experiments_number;j++)
			{
				test_netw_1.learn(learning_lps,max_epsilum,max_iters_num,batch_size);
				test_netw_1.testing(testing_lps);
				test_netw_1.print_info();
				if(test_netw_1.rounded_precision<90)
					locals_perc++;
				avg_time+=test_netw_1.time_sec;
				test_netw_1.reset();
			}

			std::cout<<"no dropout, locals percentage: "<<(locals_perc/experiments_number)*100<<" % , avg time: "<<avg_time<<" sec"<<std::endl;

			locals_perc=0;
			avg_time=0;
			test_netw_1.reset();
			for(int j=0;j<experiments_number;j++)
			{
				test_netw_1.learn(learning_lps,max_epsilum,max_iters_num,dropout_p,batch_size);
				test_netw_1.testing(testing_lps);
				test_netw_1.print_info();
				if(test_netw_1.rounded_precision<90)
					locals_perc++;
				avg_time+=test_netw_1.time_sec;
				test_netw_1.reset();
			}
			std::cout<<"dropout: "<<dropout_p<<" , locals percentage: "<<(locals_perc/experiments_number)*100<<" % , avg time: "<<avg_time<<" sec"<<std::endl;
*/

	//learn
	float experiments_number =10;
	float avg_time =0;
	//learn
	float max_iters_num = 10000;
	int batch_size = learning_lps.size();
	//max error
	float max_epsilum=0.001f;
	int start_neurons_num = 5;
	int max_adding_neurons_number = 150;
	int neurons_adding_step=1;
	std::vector<float> tested_rnd_precisions;
	std::vector<float> tested_EMS_precisions;
	std::vector<int> neurons_number;
	//NETWORK CONSTRUCTION
	mpl_network test_netw_1 = mpl_network(input_lenght,layer_description(start_neurons_num,ActFuncs::SIGM));
	//last layer's neurons number = outputs number
	//test_netw_1.push_back_layer(layer_description(start_neurons_num,ActFuncs::LINEAR));
	test_netw_1.push_back_layer(layer_description(result_vector_lenght,ActFuncs::SIGM));
	int reset_step = 5;
	//int additional_learning_step=100;
			
layer_description new_l = layer_description(neurons_adding_step,ActFuncs::SIGM);
int reset_counter=0;
//Timer
          auto start = std::chrono::high_resolution_clock::now();
		  auto stop = start;
							
     for(int i=0; i<max_adding_neurons_number/neurons_adding_step;i++)
			{
				test_netw_1.add_neurons(0,new_l);
				if(reset_counter%reset_step==0)
				{
					test_netw_1.reset();
					test_netw_1.learn(learning_lps,max_epsilum,max_iters_num,batch_size);
				}
				else
				{
			/*		test_netw_1.partly_learn(learning_lps,additional_learning_step,0,
					test_netw_1._layers[0].neurons_number-1,test_netw_1._layers[0].neurons_number-1);*/
				}
				test_netw_1.testing(testing_lps);
			//	test_netw_1.print_info();
				tested_rnd_precisions.push_back(test_netw_1.rounded_precision);
			//	tested_EMS_precisions.push_back(test_netw_1.EMS_precision);
				neurons_number.push_back(test_netw_1.total_neurons_number);
				reset_counter++;
			}
	 
 stop = std::chrono::high_resolution_clock::now();
float time_sec =((float)(std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count())/1000);
inputs_loader::save_points_to_temp(neurons_number,tested_rnd_precisions);
//inputs_loader::save_points_to_temp(neurons_number,tested_EMS_precisions);
std::cout<<std::endl<<"load finished"<<std::endl<<"experiment_time: "<<time_sec<<" sec"<<std::endl;

		/*	for(int i=0; i<(max_adding_neurons_number/neurons_adding_step);i++)
			{
				test_netw_1.add_neurons(0,new_l);
				test_netw_1.reset();
				test_netw_1.partly_learn(learning_lps,max_epsilum,max_iters_num,0,
				test_netw_1._layers[0].neurons_number-1,test_netw_1._layers[0].neurons_number-1);
			//	test_netw_1.learn(learning_lps,max_epsilum,max_iters_num);
				test_netw_1.testing(testing_lps);
				tested_rnd_precisions.push_back(test_netw_1.rounded_precision);
				tested_EMS_precisions.push_back(test_netw_1.EMS_precision);
				neurons_number.push_back(test_netw_1.total_neurons_number);
			//	test_netw_1.print_info();
			}	
			inputs_loader::save_points_to_temp(neurons_number,tested_rnd_precisions);
			std::cout<<std::endl<<"load finished"<<std::endl;

	/*		test_netw_1.remove_neurons(0,test_netw_1._layers[0].neurons_number-experiments_number,test_netw_1._layers[0].neurons_number-1);
			test_netw_1.partly_learn(learning_lps,max_epsilum,max_iters_num,0,1);
			test_netw_1.testing(testing_lps);
			test_netw_1.print_info();	
	
	/*		float locals_perc =0;
			for(int j=0;j<experiments_number;j++)
			{
				test_netw1.reset();
				test_netw1.learn(learning_lps,expected_precision,max_iters_num);
				test_netw1.testing(learning_lps,100);
				if(test_netw1.tested_precision<90)
					locals_perc++;
			}
			std::cout<<"locals percentage: "<<(locals_perc/experiments_number)*100<<" %"<<std::endl;
*/
			
   	//delete learning pairs
	for(int i=0;i<inputs_num;i++)
		{	possible_inputs[i].clear();
			MatrixOperations::delete_vector(all_targets[i]);
		}
			
	for(int i=0; i<learning_samples_num;i++)
			MatrixOperations::delete_vector(learning_input_vectors[i]);

	for(int i=0; i<testing_samples_num;i++)
			MatrixOperations::delete_vector(testing_input_vectors[i]);

	all_targets.clear();
	learning_input_vectors.clear();
	testing_input_vectors.clear();

	system("pause");

	return 0;
}
