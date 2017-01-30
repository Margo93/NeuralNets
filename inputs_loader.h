#pragma once
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <io.h>
#include <string>
#include <vector>
#include <windows.h> 
#include "LearningPair.h"

class inputs_loader
{
public:
	inputs_loader(void);
	~inputs_loader(void);
	static std::vector<std::vector<float>> get_all_input_vectors_from_files(void);
	static std::vector<float*> get_all_input_vectors(std::vector<std::vector<float>>all_input_vectors);
	static std::vector<float*> create_input_samples(std::vector<std::vector<float>>all_input_vectors,int vectors_number,float noise_level);
	static std::vector<float*> get_auto_outputs(int outputs_num, int outputs_len);
	static int get_min_inp_len(std::vector<std::vector<float>> * all_input_vectors);
	static int get_min_result_vector_lenght(int inputs_number);
	static void save_points_to_temp(std::vector<int> x, std::vector<float> y);
private:
	static std::vector<float> get_vector_from_one_file(std::string filename);
	static float* get_binary_number(int num,int num_lenght);
};

