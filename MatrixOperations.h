#pragma once
#include <vector>

class MatrixOperations
{
public:
	MatrixOperations(void);
	~MatrixOperations(void);
    static float** create_2d_matrix(int w,int h);
	static void delete_2d_matrix(float **matrix,int w,int h);
	static float* create_vector(int len);
	static void init_vector_random(float *vect,int len);
	static void  init_vector_random(std::vector<float>* vect,int len);
	static void delete_vector(float *vect);
	static void set_matrix(float **matrix,float **new_matrix_vals,int w,int h);
	static void init_matrix_random(float **matrix,int w,int h);
	static void init_matrix_random(float **matrix,int w,int h,int inp_neur_num);
	static void print_matrix(float **matrix,int w,int h);
	static void print_vector(float *vector,int len);
	static void print_vector(std::vector<float>* vect,int len);
	static void print_rounded_vector(float *vector,int len);
	static void jogging(float **matrix,int w,int h);
	static float is_not_match(float* v1,float* v2,int len);
	static float is_not_match(std::vector<float>* v1_ptr,float* v2,int len);
};

