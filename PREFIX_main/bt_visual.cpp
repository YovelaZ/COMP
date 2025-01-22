#include "bt_visual.h"

void init_bt(statistics& stat)
{
	stat.b_s_vec = new double[stat.M];
	stat.b_t_vec = new double[stat.N];

	double largest_s_bandwidth = 2.0 * stat.b_s;
	double largest_t_bandwidth = 2.0 * stat.b_t;

	for (int i_s = 0; i_s < stat.M; i_s++)
		stat.b_s_vec[i_s] = (i_s + 1) * largest_s_bandwidth / stat.M;

	for (int i_t = 0; i_t < stat.N; i_t++)
		stat.b_t_vec[i_t] = (i_t + 1) * largest_t_bandwidth / stat.N;
}

//PREFIX_tuning
void init_PREFIX_tuning(statistics& stat)
{
	prefix_plane temp_plane;
	double**temp_S;
	double**t_plane;

	for (int u = 0; u <= 2; u++)
	{
		stat.S_inter_plane_vec.push_back(t_plane);
		stat.S_plane_vec.push_back(t_plane);
		stat.S_inter_plane_vec[u] = new double*[stat.n_x];
		stat.S_plane_vec[u] = new double*[stat.n_x];
		for (int x_index = 0; x_index < stat.n_x; x_index++)
		{
			stat.S_inter_plane_vec[u][x_index] = new double[stat.n_y];
			stat.S_plane_vec[u][x_index] = new double[stat.n_y];
		}		
	}

	for (int p = 0; p < 2 * stat.n_t*stat.N; p++)
	{
		stat.p_plane_vec.push_back(temp_plane);
		for (int u = 0; u < 3; u++)
		{
			stat.p_plane_vec[p].S_plane_vec.push_back(temp_S);
			stat.p_plane_vec[p].S_plane_vec[u] = new double*[stat.n_x];
			for (int x_index = 0; x_index < stat.n_x; x_index++)
			{
				stat.p_plane_vec[p].S_plane_vec[u][x_index] = new double[stat.n_y];
				for (int y_index = 0; y_index < stat.n_y; y_index++)
					stat.p_plane_vec[p].S_plane_vec[u][x_index][y_index] = 0;
			}
		}
	}
	
	stat.win_tb_location = new int**[stat.n_t];
	for (int t_index = 0; t_index < stat.n_t; t_index++)
	{
		stat.win_tb_location[t_index] = new int*[stat.N];
		for (int b_t_index = 0; b_t_index < stat.N; b_t_index++)
			stat.win_tb_location[t_index][b_t_index] = new int[2];
	}
}

void obtain_prefix_plane_info(statistics& stat)
{
	double cur_t;
	int p = 0;

	for (int t_index = 0; t_index < stat.n_t; t_index++)
	{
		cur_t = stat.t_L + t_index * stat.incr_t;

		for (int b_t_index = 0; b_t_index < stat.N; b_t_index++)
		{
			stat.p_plane_vec[p].t_index = t_index;
			stat.p_plane_vec[p].b_t_index = stat.N - 1 - b_t_index;
			stat.p_plane_vec[p].t_value = cur_t - stat.b_t_vec[stat.p_plane_vec[p].b_t_index];
			stat.p_plane_vec[p].is_left = true;
			p++;
		}

		for (int b_t_index = 0; b_t_index < stat.N; b_t_index++)
		{
			stat.p_plane_vec[p].t_index = t_index;
			stat.p_plane_vec[p].b_t_index = b_t_index;
			stat.p_plane_vec[p].t_value = cur_t + stat.b_t_vec[stat.p_plane_vec[p].b_t_index];
			stat.p_plane_vec[p].is_left = false;
			p++;
		}
	}

	sort(stat.p_plane_vec.begin(), stat.p_plane_vec.end());
}

void obtain_tb_location(statistics& stat)
{
	int t_index;
	int b_t_index;
	bool is_left;

	for (int p = 0; p < 2 * stat.n_t*stat.N; p++)
	{
		t_index = stat.p_plane_vec[p].t_index;
		b_t_index = stat.p_plane_vec[p].b_t_index;
		is_left = stat.p_plane_vec[p].is_left;

		if (is_left == true)
			stat.win_tb_location[t_index][b_t_index][0] = p;
		else
			stat.win_tb_location[t_index][b_t_index][1] = p;
	}
}

/*void clear_PREFIX_tuning(statistics& stat)
{
	//code here
}*/

void PREFIX_tuning(statistics& stat)
{
	int cur_plane_pos;
	init_PREFIX_tuning(stat);
	obtain_prefix_plane_info(stat);
	obtain_tb_location(stat);
	
	init_Bucket(stat);

	for (int b_s_index = 0; b_s_index < stat.M; b_s_index++)
	{
		stat.b_s = stat.b_s_vec[b_s_index];
		cur_plane_pos = 0;
		stat.start_t_win_pos = 0;
		stat.end_t_win_pos = -1;

		//code here (Scan the vector of prefix planes, i.e., p_plane_vec)
		while (cur_plane_pos < 2 * stat.n_t*stat.N)
		{
			for (int i = stat.start_t_win_pos; i <= stat.n; i++)
			{
				if (i >= stat.n)
				{
					bucket_algorithm(stat, stat.S_inter_plane_vec);
					stat.start_t_win_pos = stat.n;
					stat.end_t_win_pos = stat.n - 1;
					break;
				}

				if (stat.sorted_featureVector[i][2] <= stat.p_plane_vec[cur_plane_pos].t_value)
					stat.end_t_win_pos++;
				else
				{
					//bucket_algorithm(stat, stat.p_plane_vec[cur_plane_pos].S_plane_vec);
					bucket_algorithm(stat, stat.S_inter_plane_vec);
					stat.start_t_win_pos = stat.end_t_win_pos + 1;
					break;
				}
			}

			//Compute the statistical terms for the prefix set
			if (cur_plane_pos == 0)
			{
				for (int u = 0; u <= 2; u++)
					for (int x_index = 0; x_index < stat.n_x; x_index++)
						for (int y_index = 0; y_index < stat.n_y; y_index++)
							stat.p_plane_vec[cur_plane_pos].S_plane_vec[u][x_index][y_index]
							= stat.S_inter_plane_vec[u][x_index][y_index];
			}
			else
			{
				for (int u = 0; u <= 2; u++)
					for (int x_index = 0; x_index < stat.n_x; x_index++)
						for (int y_index = 0; y_index < stat.n_y; y_index++)
							stat.p_plane_vec[cur_plane_pos].S_plane_vec[u][x_index][y_index] 
							= stat.p_plane_vec[cur_plane_pos - 1].S_plane_vec[u][x_index][y_index]
							+ stat.S_inter_plane_vec[u][x_index][y_index];
			}

			//Compute the statistical terms for the sliding window
			if (stat.p_plane_vec[cur_plane_pos].is_left == false)
			{
				int t_index = stat.p_plane_vec[cur_plane_pos].t_index;
				int b_t_index = stat.p_plane_vec[cur_plane_pos].b_t_index;
				int left_plane_pos = stat.win_tb_location[t_index][b_t_index][0];
				double b_t_value = stat.b_t_vec[b_t_index];
				double t_value = stat.t_L + t_index * stat.incr_t; 
				//= stat.p_plane_vec[cur_plane_pos].t_value;
				double coeff_S_W0, coeff_S_W1, coeff_S_W2;

				for (int u = 0; u <= 2; u++)
					for (int x_index = 0; x_index < stat.n_x; x_index++)
						for (int y_index = 0; y_index < stat.n_y; y_index++)
							stat.S_plane_vec[u][x_index][y_index]
							= stat.p_plane_vec[cur_plane_pos].S_plane_vec[u][x_index][y_index]
							- stat.p_plane_vec[left_plane_pos].S_plane_vec[u][x_index][y_index];

				coeff_S_W0 = 1.0 - (t_value * t_value) / (b_t_value*b_t_value);
				coeff_S_W1 = (2.0 * t_value) / (b_t_value*b_t_value);
				coeff_S_W2 = -1.0 / (b_t_value*b_t_value);

				//output the results
				for (int x_index = 0; x_index < stat.n_x; x_index++)
					for (int y_index = 0; y_index < stat.n_y; y_index++)
						stat.out_multiple_tensors[b_s_index][b_t_index][x_index][y_index][t_index]
						= coeff_S_W0 * stat.S_plane_vec[0][x_index][y_index]
						+ coeff_S_W1 * stat.S_plane_vec[1][x_index][y_index]
						+ coeff_S_W2 * stat.S_plane_vec[2][x_index][y_index];
			}

			cur_plane_pos++;
		}

		//clear_PREFIX_tuning(stat);
	}
	erase_Bucket(stat);
}

void SWS_bt(statistics& stat)
{
	//stat.gamma_s = 1.0 / stat.b_s;
	//stat.gamma_t = 1.0 / stat.b_t;
	init_SWS(stat);

	for (int b_s_index = 0; b_s_index < stat.M; b_s_index++)
	{
		stat.b_s_index = b_s_index;
		stat.b_s = stat.b_s_vec[b_s_index];
		stat.gamma_s = 1.0 / stat.b_s;

		for (int b_t_index = 0; b_t_index < stat.N; b_t_index++)
		{
			stat.b_t_index = b_t_index;
			stat.b_t = stat.b_t_vec[b_t_index];
			stat.gamma_t = 1.0 / stat.b_t;

			for (int x_index = 0; x_index < stat.n_x; x_index++)
				for (int y_index = 0; y_index < stat.n_y; y_index++)
					SWS_single_pixel(x_index, y_index, stat, stat.b_s, stat.b_t);
		}
	}
}

//Used for checking the correctness
void SS_bt(statistics& stat)
{
	for (int b_s_index = 0; b_s_index < stat.M; b_s_index++)
	{
		stat.b_s = stat.b_s_vec[b_s_index];
		for (int b_t_index = 0; b_t_index < stat.N; b_t_index++)
		{
			stat.b_t = stat.b_t_vec[b_t_index];
			for (int x_index = 0; x_index < stat.n_x; x_index++)
			{
				stat.q[0] = stat.x_L + x_index * stat.incr_x;
				for (int y_index = 0; y_index < stat.n_y; y_index++)
				{
					stat.q[1] = stat.y_L + y_index * stat.incr_y;
					for (int t_index = 0; t_index < stat.n_t; t_index++)
					{
						stat.q[2] = stat.t_L + t_index * stat.incr_t;
						stat.out_multiple_tensors[b_s_index][b_t_index][x_index][y_index][t_index] = SCAN(stat);
					}
				}
			}
		}
	}	
}