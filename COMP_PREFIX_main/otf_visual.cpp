#include "otf_visual.h"

void init_otf(statistics& stat)
{
	double random_ratio;
	stat.timestamp_vec = new double[stat.n_t];
	default_random_engine generator;
	uniform_real_distribution<double> distribution(0.0, 1.0);

	for (int t_index = 0; t_index < stat.n_t; t_index++)
	{
		random_ratio = distribution(generator);
		stat.timestamp_vec[t_index] = stat.t_L + random_ratio * (stat.t_U - stat.t_L);
	}

	for (int i = 0; i < stat.n; i++)
		stat.sorted_fV_timestamp_vec.push_back(stat.sorted_featureVector[i][2]);
}

void SS_otf(statistics& stat)
{
	for (int x_index = 0; x_index < stat.n_x; x_index++)
	{
		stat.q[0] = stat.x_L + x_index * stat.incr_x;
		for (int y_index = 0; y_index < stat.n_y; y_index++)
		{
			stat.q[1] = stat.y_L + y_index * stat.incr_y;
			for (int t_index = 0; t_index < stat.n_t; t_index++)
			{
				stat.q[2] = stat.timestamp_vec[t_index];
				stat.out_tensor[x_index][y_index][t_index] = SCAN(stat);
			}
		}
	}
}

void SWS_otf(statistics& stat)
{
	vector<double>::iterator left, right;

	for (int t_index = 0; t_index < stat.n_t; t_index++)
	{
		stat.q[2] = stat.timestamp_vec[t_index];
		//Find the lower and upper bound of the time window
		left = lower_bound(stat.sorted_fV_timestamp_vec.begin(),
			stat.sorted_fV_timestamp_vec.end(), stat.timestamp_vec[t_index] - stat.b_t);
		right = upper_bound(stat.sorted_fV_timestamp_vec.begin(),
			stat.sorted_fV_timestamp_vec.end(), stat.timestamp_vec[t_index] + stat.b_t);
		
		stat.start_t_win_pos = left - stat.sorted_fV_timestamp_vec.begin();
		stat.end_t_win_pos = min((int)(right - stat.sorted_fV_timestamp_vec.begin()), stat.n) - 1;

		for (int x_index = 0; x_index < stat.n_x; x_index++)
		{
			stat.q[0] = stat.x_L + x_index * stat.incr_x;
			for (int y_index = 0; y_index < stat.n_y; y_index++)
			{
				stat.q[1] = stat.y_L + y_index * stat.incr_y;
				stat.out_tensor[x_index][y_index][t_index] = SCAN_W_t(stat);
			}
		}
	}
}

//PREFIX_single
void init_PREFIX_single(statistics& stat)
{
	double**temp_Plane;
	for (int u = 0; u <= 2; u++)
	{
		stat.S_plane_vec.push_back(temp_Plane);
		stat.S_plane_vec[u] = new double*[stat.n_x];
		for (int x_index = 0; x_index < stat.n_x; x_index++)
		{
			stat.S_plane_vec[u][x_index] = new double[stat.n_y];
			for (int y_index = 0; y_index < stat.n_y; y_index++)
				stat.S_plane_vec[u][x_index][y_index] = 0;
		}
	}
}

void clear_PREFIX_single(statistics& stat)
{
	for (int u = 0; u <= 2; u++)
		for (int x_index = 0; x_index < stat.n_x; x_index++)
			for (int y_index = 0; y_index < stat.n_y; y_index++)
				stat.S_plane_vec[u][x_index][y_index] = 0;
}

void PREFIX_single(statistics& stat)
{
	vector<double>::iterator left, right;      
	double coeff_S_W0, coeff_S_W1, coeff_S_W2;

	init_PREFIX_single(stat);

	for (int t_index = 0; t_index < stat.n_t; t_index++)
	{
		//Find the lower and upper bound of the time window
		left = lower_bound(stat.sorted_fV_timestamp_vec.begin(),
			stat.sorted_fV_timestamp_vec.end(), stat.timestamp_vec[t_index] - stat.b_t);
		right = upper_bound(stat.sorted_fV_timestamp_vec.begin(),
			stat.sorted_fV_timestamp_vec.end(), stat.timestamp_vec[t_index] + stat.b_t);

		stat.start_t_win_pos = left - stat.sorted_fV_timestamp_vec.begin();
		stat.end_t_win_pos = min((int)(right - stat.sorted_fV_timestamp_vec.begin()), stat.n) - 1;

		coeff_S_W0 = 1.0 - (stat.timestamp_vec[t_index] * stat.timestamp_vec[t_index]) / (stat.b_t*stat.b_t);
		coeff_S_W1 = 2.0 * stat.timestamp_vec[t_index] / (stat.b_t*stat.b_t);
		coeff_S_W2 = -1.0 / (stat.b_t*stat.b_t);

		init_Bucket(stat);
		bucket_algorithm(stat, stat.S_plane_vec);
		erase_Bucket(stat);

		for (int x_index = 0; x_index < stat.n_x; x_index++)
			for (int y_index = 0; y_index < stat.n_y; y_index++)
				stat.out_tensor[x_index][y_index][t_index] = coeff_S_W0 * stat.S_plane_vec[0][x_index][y_index]
				+ coeff_S_W1 * stat.S_plane_vec[1][x_index][y_index]
				+ coeff_S_W2 * stat.S_plane_vec[2][x_index][y_index];

		clear_PREFIX_single(stat);
	}
}