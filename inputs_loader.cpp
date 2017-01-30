#include "stdafx.h"
#include "inputs_loader.h"
#include "MatrixOperations.h"
#include <time.h>


inputs_loader::inputs_loader(void)
{

}


inputs_loader::~inputs_loader(void)
{

}

std::vector<float*> inputs_loader::get_all_input_vectors(std::vector<std::vector<float>> all_input_vectors)
{
	int vecs_num = all_input_vectors.size();
	std::vector<float*> input_vectors;
	input_vectors.reserve(vecs_num);
	int len;
	for(int inp=0; inp<vecs_num;inp++)
  {
	len=all_input_vectors[inp].size();
	input_vectors.push_back(MatrixOperations::create_vector(len));
	for(int i=0;i<len;i++)
	input_vectors[inp][i]=all_input_vectors[inp][i];
   }
	return input_vectors;
}

std::vector<float*> inputs_loader::create_input_samples(std::vector<std::vector<float>>all_input_vectors,int vectors_number,float noise_level)
{
	int samples_num = all_input_vectors.size();
	std::vector<float*> result_vectors;
	result_vectors.reserve(vectors_number);
	int len = all_input_vectors[0].size();
	int inp=0;
	srand(time(0));
	int pixels_for_changing_num =(int)(noise_level*float(len));
	int changing_pixel_id =0;
	float noised_pixel_mean =0;

	for(int s=0; s<vectors_number;s++)
	{
		inp = s%samples_num;
		len=all_input_vectors[inp].size();
		result_vectors.push_back(MatrixOperations::create_vector(len));

		for(int i=0;i<len;i++)
		result_vectors[s][i]=all_input_vectors[inp][i];

			 for(int i=0; i<pixels_for_changing_num;i++)
	 {
		 changing_pixel_id=(rand()%len);
		 noised_pixel_mean = (float)(rand()%100)/1000;
		 result_vectors[s][i] = noised_pixel_mean;
	 }

   }

	return result_vectors;
}


std::vector<float*> inputs_loader::get_auto_outputs(int outputs_num,int outputs_len)
{
	std::vector<float*> targets;
	targets.reserve(outputs_num);
	for(int i=0;i<outputs_num;i++)
		targets.push_back(get_binary_number((i+1),outputs_len));

	return targets;
}

float* inputs_loader::get_binary_number(int num,int num_lenght)
{
	float* result=MatrixOperations::create_vector(num_lenght);
	for(int i=0;i<num_lenght;i++)
		result[i]=0;

	std::vector<float> reverse_bin_num;
	do{
		reverse_bin_num.push_back(num%2);
		num/=2;
      }while (num!=0);

	int bin_num_len = reverse_bin_num.size();

	for(int i=0;i<bin_num_len;i++)
		result[num_lenght-i-1]=reverse_bin_num[i];

	return result;
}

int inputs_loader::get_min_inp_len(std::vector<std::vector<float>> * all_input_vectors)
{ int min_len = (*all_input_vectors)[0].size();
  int cur_len = min_len;
  for(int i=0; i<all_input_vectors->size();i++)
	  {
		  cur_len= (*all_input_vectors)[i].size();
		  if(cur_len<min_len)
			  min_len=cur_len;
	  }
  return min_len;
}

int inputs_loader::get_min_result_vector_lenght(int inputs_number)
{
	//numeration starts with 0...01
	int res = std::log((inputs_number+1))/std::log(2);
	//rounding up 
	res++;
	return res;
}

std::vector<std::vector<float>> inputs_loader::get_all_input_vectors_from_files(void)
{
	setlocale(LC_ALL, "rus");
    WIN32_FIND_DATA findf;
	LPCWSTR str = _T("InputTxts/*.txt");
	std::string filename="";
	char ch=' ';
	int id=0;

	std::vector<std::vector<float>> all_inp_vectors;

    HANDLE hFind = FindFirstFile(str, &findf);
		while(ch!='\0')
	   {
			ch=(char)(findf.cFileName[id]);
			filename+= ch;
			id++;
		}

		all_inp_vectors.push_back(get_vector_from_one_file("InputTxts/"+filename));

    while (FindNextFile(hFind, &findf)){
		filename="";
		ch=' ';
		id=0;
	while(ch!='\0')
		{
			ch=(char)(findf.cFileName[id]);
			filename+= ch;
			id++;
		}
		all_inp_vectors.push_back(get_vector_from_one_file("InputTxts/"+filename));
    }
//end search
    FindClose(hFind);	

	return all_inp_vectors;
}

std::vector<float> inputs_loader::get_vector_from_one_file(std::string filename)
{
	std::ifstream fin;
	std::string s="";
	std::string temp;

	fin.open(filename,std::ios_base::in);
	while(fin>>temp)
		s+=temp;
    fin.close();

	std::vector<float> res;
	int strid=0;
	int strlen=s.size();
	temp="";
	
	while (strid<strlen)
	{

		while(s[strid]!=',')
		{
			temp+=s[strid];
		  strid++;
		}
		//get img pixels,[0...255] grey colors
		res.push_back(atof(temp.c_str())/255);

		temp="";
	strid++;
	}

	//input normalizetion
	for(int i=0; i< res.size();i++)
		res[i] = res[i]/res.size();
	return res;
}

void inputs_loader::save_points_to_temp(std::vector<int> x, std::vector<float> y)
{
	
	std::ofstream fout;
	int min_len = std::min<int>(x.size(),y.size());
	fout.open("OutputTxts/Temp.txt",std::ios_base::out|std::ios_base::app);
	for(int i=0;i<min_len;i++)
	{
		fout<<"("<<x[i]<<";"<<y[i]<<")";
	}
    fout.close();	
}