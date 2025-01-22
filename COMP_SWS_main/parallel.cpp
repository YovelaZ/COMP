#include "parallel.h"

void init_parallel(statistics& stat, statistics* stat_thread_vec)
{
	for (int tid = 0; tid < stat.thread_num; tid++)
	{
		stat_thread_vec[tid].n = stat.n;
		stat_thread_vec[tid].n_x = stat.n_x; stat_thread_vec[tid].n_y = stat.n_y; stat_thread_vec[tid].n_t = stat.n_t;
		stat_thread_vec[tid].x_L = stat.x_L; stat_thread_vec[tid].x_U = stat.x_U; 
		stat_thread_vec[tid].y_L = stat.y_L; stat_thread_vec[tid].y_U = stat.y_U; 
		stat_thread_vec[tid].t_L = stat.t_L; stat_thread_vec[tid].t_U = stat.t_U;
		stat_thread_vec[tid].incr_x = stat.incr_x; stat_thread_vec[tid].incr_y = stat.incr_y; stat_thread_vec[tid].incr_t = stat.incr_t;
		stat_thread_vec[tid].method = stat.method; 
		stat_thread_vec[tid].k_type_s = stat.k_type_s;
		stat_thread_vec[tid].k_type_t = stat.k_type_t;
		stat_thread_vec[tid].gamma_ratio_s = stat.gamma_ratio_s;
		stat_thread_vec[tid].gamma_ratio_t = stat.gamma_ratio_t;
		stat_thread_vec[tid].gamma_s = stat.gamma_s;
		stat_thread_vec[tid].gamma_t = stat.gamma_t;
		stat_thread_vec[tid].b = stat.b;
		stat_thread_vec[tid].q = new double[stat.dim];
		stat_thread_vec[tid].featureVector = stat.featureVector;
		stat_thread_vec[tid].out_tensor = stat.out_tensor;
		stat_thread_vec[tid].sorted_featureVector = stat.sorted_featureVector;
		stat_thread_vec[tid].thread_num = stat.thread_num;

		if (stat.method == 6) //RQS (kd-tree)
		{
			stat_thread_vec[tid].query_boundary = new double*[stat.dim];
			for (int d = 0; d < stat.dim; d++)
				stat_thread_vec[tid].query_boundary[d] = new double[2];
		}

		if (stat.method == 8) //SWS
		{
			if (stat.k_type_t == 0) //Triangular kernel
			{
				stat_thread_vec[tid].sliding_window_L = new double[2];
				stat_thread_vec[tid].sliding_window_R = new double[2];
			}
			if (stat.k_type_t == 1) //Epanechnikov kernel
				stat_thread_vec[tid].sliding_window = new double[3];
			if (stat.k_type_t == 2) //Quartic kernel
				stat_thread_vec[tid].sliding_window = new double[5];
		}
	}
}

void thread_parallel(int tid, statistics& stat_thread)
{
	int x_index, y_index, t_index;
	int total_num_entries;
	int num_entries_per_thread;
	int start_entry;
	int end_entry;
	double s_bandwidth, t_bandwidth;

	kdTree kd_tree(stat_thread.dim, stat_thread.featureVector, leafCapacity);
	ballTree ball_tree(stat_thread.dim, stat_thread.featureVector, leafCapacity);

	if (stat_thread.method == 6) //RQS (kd-tree)
	{
		stat_thread.query_boundary = new double*[stat_thread.dim];
		for (int d = 0; d < stat_thread.dim; d++)
			stat_thread.query_boundary[d] = new double[2];
		kd_tree.rootNode = new kdNode();
		kd_tree.build_kdTree(stat_thread);
	}

	if (stat_thread.method == 7 || stat_thread.method == 8) //SWS for method = 8, RQS (ball-tree) for method = 7
	{
		s_bandwidth = 1.0 / stat_thread.gamma_s;
		t_bandwidth = 1.0 / stat_thread.gamma_t;

		if (stat_thread.method == 7)
		{
			ball_tree.rootNode = new ballNode();
			ball_tree.build_ballTree(stat_thread);
		}
	}

	if (stat_thread.method >= 5 && stat_thread.method <= 7)
	{
		total_num_entries = stat_thread.n_x * stat_thread.n_y * stat_thread.n_t;
		num_entries_per_thread = (int)ceil((double)(total_num_entries) / stat_thread.thread_num);
		start_entry = tid * num_entries_per_thread;
		end_entry = (tid + 1)*num_entries_per_thread;

		for (int e = start_entry; e < min(end_entry, total_num_entries); e++)
		{
			x_index = (int)floor((double)e / (stat_thread.n_y * stat_thread.n_t));
			y_index = (int)floor((double)(e - x_index * (stat_thread.n_y * stat_thread.n_t)) / stat_thread.n_t);
			t_index = e - x_index * (stat_thread.n_y * stat_thread.n_t) - y_index * stat_thread.n_t;

			obtain_q(x_index, y_index, t_index, stat_thread);

			if (stat_thread.method == 5) //SCAN
				stat_thread.out_tensor[x_index][y_index][t_index] = SCAN(stat_thread);
			if (stat_thread.method == 6) //RAQ with kd-tree
			{
				kd_tree.obtain_boundary(stat_thread);
				kd_tree.range_search(stat_thread.query_boundary);
				stat_thread.out_tensor[x_index][y_index][t_index] = SCAN_RQS_result(stat_thread, kd_tree);
				kd_tree.range_result_idList.clear();
			}
			if (stat_thread.method == 7) //RAQ with ball-tree
			{
				ball_tree.range_search(stat_thread.q, s_bandwidth, t_bandwidth);
				stat_thread.out_tensor[x_index][y_index][t_index] = SCAN_RQS_result(stat_thread, ball_tree);
				ball_tree.range_result_idList.clear();
			}
		}
	}

	if (stat_thread.method == 8) //SWS
	{
		total_num_entries = stat_thread.n_x * stat_thread.n_y;
		num_entries_per_thread = (int)ceil((double)(total_num_entries) / stat_thread.thread_num);
		start_entry = tid * num_entries_per_thread;
		end_entry = (tid + 1)*num_entries_per_thread;

		for (int e = start_entry; e < min(end_entry, total_num_entries); e++)
		{
			x_index = (int)floor((double)e / stat_thread.n_y);
			y_index = e - x_index * stat_thread.n_y;

			SWS(x_index, y_index, stat_thread, s_bandwidth, t_bandwidth);
		}
	}
}

void STKDV_parallel(statistics& stat)
{
	statistics*stat_thread_vec;
	thread*th_vec = new thread[stat.thread_num];
	stat_thread_vec = new statistics[stat.thread_num];

	//code here
	init_parallel(stat, stat_thread_vec);

	for (int tid = 0; tid < stat.thread_num; tid++)
		th_vec[tid] = thread(thread_parallel, tid, ref(stat_thread_vec[tid]));

	for (int tid = 0; tid < stat.thread_num; tid++)
		th_vec[tid].join();
}