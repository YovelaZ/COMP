#pragma once
#ifndef INIT_VISUAL_H
#define INIT_VISUAL_H

#include "Library.h"

const double inf = 999999999999;
const double small_epsilon = 0.01;
const double pi = 3.14159265358979323846;
const int dim = 3; //dim = 3 (3d visualization)

//#define OBTAIN_BANDWIDTHS

struct prefix_plane
{
	vector<double**> S_plane_vec;
	int t_index;
	int b_t_index;
	double t_value;
	bool is_left;
	//int left_end_pos;
	bool operator<(const prefix_plane& p) { return t_value < p.t_value; }
};

struct statistics
{
	int STKDV_type; //0: STKDV with on-the-fly timestamps, 1: STKDV with known timestamps, 2: STKDV with bandwidth tuning
	int n; //number of points in datasets
	int n_x; //number of discrete region in the x-axis, e.g., 1280
	int n_y; //number of discrete region in the y-axis, e.g., 960
	int n_t; //number of discrete region in the t-axis, e.g., 32
	double x_L, x_U; //lower and upper region of data points in x-axis
	double y_L, y_U; //lower and upper region of data points in y-axis
	double t_L, t_U; //lower and upper region of data points in t-axis
	double incr_x; //incremental value in the x-axis
	double incr_y; //incremental value in the y-axis
	double incr_t; //incremental value in the t-axis
	int method; //chosen method
	int k_type_s; //kernel type 1: Epanechnikov kernel, 2: Quartic kernel, 3: Uniform kernel
	int k_type_t; //kernel type 1: Epanechnikov kernel, 2: Quartic kernel, 3: Uniform kernel
	double b_s_ratio; //control the bandwidth of spatial kernel
	double b_t_ratio; //control the bandwidth of temporal kernel
	double b_s; //The bandwidth of spatial kernel
	double b_t; //The bandwidth of temporal kernel
	char*dataFileName;
	char*outputFileName;
	bool is_default_region;

	double*q;
	double**featureVector; //feature vector of all data points (d = 0, 1, and 2 represent x, y, and timestamp, respectively)
	double**sorted_featureVector; //sorted according to the timestamp parameter
	double***out_tensor; //output visualization
    double runtime;

	//Bucket-based algorithm
	vector< vector<int> > B_L;
	vector< vector<int> > B_U;
	double*C_L; double*C_U;
	double**v_L; double**v_U;
	double*H_L; double*H_U;
	double*C_R_q;
	double**v_R_q;
	double*H_R_q;

	//Used in PREFIX_single, PREFIX_multiple, and PREFIX_tuning (store the plane of the statistical terms)
	vector<double**> S_plane_vec;
	vector<double> sorted_fV_timestamp_vec;

	//STKDV_type = 0 (on-the-fly timestamps)
	double*timestamp_vec; //store n_t timestamps 
	int start_t_win_pos;
	int end_t_win_pos;

	//STKDV_type = 1 (known timestamps)
	//Used in PREFIX_multiple (store the plane of the statistical terms)
	vector<double**> S_D_plane_vec;
	vector<double**> S_I_plane_vec;
	int start_S_D_t_win_pos;
	int end_S_D_t_win_pos;
	int start_S_I_t_win_pos;
	int end_S_I_t_win_pos;

	//STKDV_type = 2 (bandwidth tuning)
	int M;
	int N;
	double*b_s_vec;
	double*b_t_vec;
	double*****out_multiple_tensors;
	vector<prefix_plane> p_plane_vec;
	vector<double**> S_inter_plane_vec;
	int***win_tb_location;
	//vector<double**> S_plane_vec;
	//double*p_plane_t_vec;

	//Used in the SWS method
	double*sliding_window;
	double gamma_s; //Control the bandwidth of spatial kernel
	double gamma_t; //Control the bandwidth of temporal kernel
	int b_s_index;
	int b_t_index;
};

struct index_time_pair
{
	int index;
	double time;
	bool operator<(const index_time_pair& pair) { return time < pair.time; }
};

void initTensor(statistics& stat);
void initStat(int argc, char**argv, statistics& stat);
void extract_FeatureVector(statistics& stat);
void sort_FeatureVector(statistics& stat);
void updateRegion_default(statistics& stat);
//void updateRegion(statistics& stat);
void update_incr_value(statistics& stat);
void obtain_bandwidth_values(statistics& stat);
void init_visual(int argc, char**argv, statistics& stat);
void save_result_to_file(statistics& stat);

#endif