#include "SS.h"

double spatial_kernel(double*q, double*p, statistics& stat)
{
	double value = 1 - (1 / (stat.b_s*stat.b_s))*((q[0] - p[0])*(q[0] - p[0]) + (q[1] - p[1])*(q[1] - p[1]));

	if (stat.k_type_s == 1) //Epanechnikov kernel
	{
		if (value < 0)
			return 0;
		else
			return value;
	}

	if (stat.k_type_s == 2) //Quartic kernel
	{
		if (value < 0)
			return 0;
		else
			return value * value;
	}

	if (stat.k_type_s == 3) //Uniform kernel
	{
		if (value < 0)
			return 0;
		else
			return 1;
	}

	return -inf;
}

double temporal_kernel(double*q, double*p, statistics& stat)
{
	double value = 1 - (1 / (stat.b_t*stat.b_t))*(q[2] - p[2])*(q[2] - p[2]);

	if (stat.k_type_t == 1) //Epanechnikov kernel
	{
		if (value < 0)
			return 0;
		else
			return value;
	}

	if (stat.k_type_t == 2) //Quartic kernel
	{
		if (value < 0)
			return 0;
		else
			return value * value;
	}

	if (stat.k_type_t == 2) //Uniform kernel
	{
		if (value < 0)
			return 0;
		else
			return 1;
	}

	return -inf;
}

double SCAN_W_t(statistics& stat)
{
	double incr_value = 0;

	for (int i = stat.start_t_win_pos; i <= stat.end_t_win_pos; i++)
		incr_value += spatial_kernel(stat.q, stat.sorted_featureVector[i], stat)*temporal_kernel(stat.q, stat.sorted_featureVector[i], stat);

	return incr_value;
}

double SCAN(statistics& stat)
{
	double incr_value;

	incr_value = 0;
	for (int i = 0; i < stat.n; i++)
		incr_value += spatial_kernel(stat.q, stat.featureVector[i], stat)*temporal_kernel(stat.q, stat.featureVector[i], stat);

	return incr_value;
}