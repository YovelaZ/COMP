#include "init_visual.h"

void initTensor(statistics& stat)
{
	if (stat.STKDV_type == 0 || stat.STKDV_type == 1)
	{
		stat.out_tensor = new double**[stat.n_x];
		for (int x = 0; x < stat.n_x; x++)
		{
			stat.out_tensor[x] = new double*[stat.n_y];
			for (int y = 0; y < stat.n_y; y++)
				stat.out_tensor[x][y] = new double[stat.n_t];
		}
	}

	if (stat.STKDV_type == 2)
	{
		stat.out_multiple_tensors = new double****[stat.M];
		for (int s_index = 0; s_index < stat.M; s_index++)
		{
			stat.out_multiple_tensors[s_index] = new double***[stat.N];
			for (int t_index = 0; t_index < stat.N; t_index++)
			{
				stat.out_multiple_tensors[s_index][t_index] = new double**[stat.n_x];
				for (int x_index = 0; x_index < stat.n_x; x_index++)
				{
					stat.out_multiple_tensors[s_index][t_index][x_index] = new double*[stat.n_y];
					for (int y_index = 0; y_index < stat.n_y; y_index++)
						stat.out_multiple_tensors[s_index][t_index][x_index][y_index] = new double[stat.n_t];
				}
			}
		}
	}
}

void initStat(int argc, char**argv, statistics& stat)
{
	stat.dataFileName = argv[1];
	stat.outputFileName = argv[2];
	stat.STKDV_type = atoi(argv[3]);
	stat.method = atoi(argv[4]);
	stat.n_x = atoi(argv[5]);
	stat.n_y = atoi(argv[6]);
	stat.n_t = atoi(argv[7]);
	stat.k_type_s = atoi(argv[8]);
	stat.k_type_t = atoi(argv[9]);
	stat.b_s_ratio = atof(argv[10]);
	stat.b_t_ratio = atof(argv[11]);
	stat.is_default_region = (bool)atoi(argv[12]);

	if (stat.is_default_region == false)
	{
		stat.x_L = atof(argv[13]);
		stat.x_U = atof(argv[14]);
		stat.y_L = atof(argv[15]);
		stat.y_U = atof(argv[16]);
		stat.t_L = atof(argv[17]);
		stat.t_U = atof(argv[18]);

		if (stat.STKDV_type == 2) //bandwidth tuning
		{
			stat.M = atoi(argv[19]);
			stat.N = atoi(argv[20]);
		}
	}
	else
	{
		if (stat.STKDV_type == 2) //bandwidth tuning
		{
			stat.M = atoi(argv[13]);
			stat.N = atoi(argv[14]);
		}
	}
    stat.runtime = 0;
	//debug
	/*stat.dataFileName = (char*)"../../../Datasets/New_York_Taxi/New_York_Taxi_s_25";
	stat.outputFileName = (char*)"./Results/Case_Study/New_York_Taxi_S1_MD2_bs_270_bt_0_2";
	stat.STKDV_type = 1;
	stat.method = 2;
	stat.n_x = 320;
	stat.n_y = 240;
	stat.n_t = 32;
	stat.k_type_s = 1;
	stat.k_type_t = 1;
	stat.b_s_ratio = 1;
	stat.b_t_ratio = 0.60219;
	stat.is_default_region = false;
	stat.x_L = -18570.0;
	stat.x_U = -2010.0;
	stat.y_L = -12260.0;
	stat.y_U = 6260.0;
	stat.t_L = 0.282407407;
	stat.t_U = 31.28239503;*/

	/*stat.dataFileName = (char*)"../../../Datasets/Testing/Testing";
	stat.outputFileName = (char*)"./Results/Testing_S1_M0_X8_Y5_T4_BS_4_BT_2";
	stat.STKDV_type = 1;
	stat.method = 0;
	stat.n_x = 8;
	stat.n_y = 5;
	stat.n_t = 4;
	stat.M = 4;
	stat.N = 2;
	stat.k_type_s = 1;
	stat.k_type_t = 1;
	stat.b_s_ratio = 2;
	stat.b_t_ratio = 2;*/
}

void extract_FeatureVector(statistics& stat)
{
	//load data to feature array
	fstream file;
	string temp_string;

	file.open(stat.dataFileName);
	if (file.is_open() == false)
	{
		cout << "Cannot Open File!" << endl;
		exit(1);
	}

//	file >> temp_string;
	file >> stat.n;
    cout << stat.n << endl;

    double h_s, h_t;
    file >> h_s;
    file >> h_t;

    stat.b_s = stat.b_s_ratio*h_s;
    stat.b_t = stat.b_t_ratio*h_t;

    cout << stat.b_s << endl;
    cout << stat.b_t << endl;

	stat.featureVector = new double*[stat.n];
	for (int i = 0; i < stat.n; i++)
		stat.featureVector[i] = new double[dim+1];

//	file >> temp_string; file >> temp_string; file >> temp_string;
	for (int i = 0; i < stat.n; i++)
		for (int d = 0; d < dim+1; d++)
			file >> stat.featureVector[i][d];

    cout << stat.featureVector[0][0] << endl;
    cout << stat.featureVector[0][1] << endl;
    cout << stat.featureVector[0][2] << endl;
	cout << stat.featureVector[0][3] << endl;

	file.close();
}

void sort_FeatureVector(statistics& stat)
{
	vector<index_time_pair> pair_vector;
	index_time_pair pair;
	int cur_index;

	stat.sorted_featureVector = new double*[stat.n];
	for (int i = 0; i < stat.n; i++)
	{
		pair.index = i;
		pair.time = stat.featureVector[i][2];
		pair_vector.push_back(pair);
		stat.sorted_featureVector[i] = new double[dim+1];
	}

	sort(pair_vector.begin(), pair_vector.end());

	for (int i = 0; i < stat.n; i++)
	{
		cur_index = pair_vector[i].index;
		for (int d = 0; d < dim+1; d++)
			stat.sorted_featureVector[i][d] = stat.featureVector[cur_index][d];
	}
}

void updateRegion_default(statistics& stat)
{
	stat.x_L = inf; stat.x_U = -inf;
	stat.y_L = inf;	stat.y_U = -inf;
	stat.t_L = inf;	stat.t_U = -inf;

	for (int i = 0; i < stat.n; i++)
	{
		if (stat.featureVector[i][0] < stat.x_L)
			stat.x_L = stat.featureVector[i][0];
		if (stat.featureVector[i][0] > stat.x_U)
			stat.x_U = stat.featureVector[i][0];

		if (stat.featureVector[i][1] < stat.y_L)
			stat.y_L = stat.featureVector[i][1];
		if (stat.featureVector[i][1] > stat.y_U)
			stat.y_U = stat.featureVector[i][1];

		if (stat.featureVector[i][2] < stat.t_L)
			stat.t_L = stat.featureVector[i][2];
		if (stat.featureVector[i][2] > stat.t_U)
			stat.t_U = stat.featureVector[i][2];
	}
}

void update_incr_value(statistics& stat)
{
	stat.q = new double[dim];
	stat.incr_x = (stat.x_U - stat.x_L) / stat.n_x;
	stat.incr_y = (stat.y_U - stat.y_L) / stat.n_y;
	stat.incr_t = (stat.t_U - stat.t_L) / stat.n_t;
}

void obtain_bandwidth_values(statistics& stat)
{
	double sum_x, sum_y, sum_t;
	double mean_x, mean_y, mean_t;
	double sd_x, sd_y, sd_t;
	double h_x, h_y, h_t;

	//Using Scott's rule to obtain the bandwidth for both spatial and temporal kernels

	//Compute mean value
	sum_x = 0; sum_y = 0; sum_t = 0;
	for (int i = 0; i < stat.n; i++)
	{
		sum_x += stat.featureVector[i][0];
		sum_y += stat.featureVector[i][1];
		sum_t += stat.featureVector[i][2];
	}
	mean_x = sum_x / stat.n; mean_y = sum_y / stat.n; mean_t = sum_t / stat.n;

	sd_x = 0; sd_y = 0; sd_t = 0;
	for (int i = 0; i < stat.n; i++)
	{
		sd_x += (stat.featureVector[i][0] - mean_x)*(stat.featureVector[i][0] - mean_x) / (stat.n - 1);
		sd_y += (stat.featureVector[i][1] - mean_y)*(stat.featureVector[i][1] - mean_y) / (stat.n - 1);
		sd_t += (stat.featureVector[i][2] - mean_t)*(stat.featureVector[i][2] - mean_t) / (stat.n - 1);
	}
	sd_x = sqrt(sd_x); sd_y = sqrt(sd_y); sd_t = sqrt(sd_t);

	h_x = pow((double)stat.n, -1.0 / 6.0)*sd_x; //d = 2
	h_y = pow((double)stat.n, -1.0 / 6.0)*sd_y; //d = 2
	h_t = pow((double)stat.n, -1.0 / 5.0)*sd_t; //d = 1

	stat.b_s = stat.b_s_ratio*sqrt(h_x*h_x + h_y * h_y);
	stat.b_t = stat.b_t_ratio*h_t;

	#ifdef OBTAIN_BANDWIDTHS
	cout << "default spatial bandwidth:" << stat.b_s << endl;
	cout << "default temporal bandwidth:" << stat.b_t << endl;
	exit(0);
	#endif
}

void init_visual(int argc, char**argv, statistics& stat)
{
	initStat(argc, argv, stat);
	initTensor(stat);
	extract_FeatureVector(stat);
//	obtain_bandwidth_values(stat);
	if (stat.is_default_region == true)
		updateRegion_default(stat); //Use the default region
	update_incr_value(stat);
	sort_FeatureVector(stat);
}

void save_result_to_file(statistics& stat)
{
	fstream outputFile;
	outputFile.open(stat.outputFileName, ios::in | ios::out | ios::trunc);

	outputFile << "x_L " << stat.x_L << endl;
	outputFile << "x_U " << stat.x_U << endl;
	outputFile << "y_L " << stat.y_L << endl;
	outputFile << "y_U " << stat.y_U << endl;
	outputFile << "t_L " << stat.t_L << endl;
	outputFile << "t_U " << stat.t_U << endl;
	outputFile << "n_x " << stat.n_x << endl;
	outputFile << "n_y " << stat.n_y << endl;
	outputFile << "n_t " << stat.n_t << endl;

	if (stat.STKDV_type == 0 || stat.STKDV_type == 1)
	{
		for (int t = 0; t < stat.n_t; t++)
			for (int y = 0; y < stat.n_y; y++)
				for (int x = 0; x < stat.n_x; x++)
					outputFile << stat.out_tensor[x][y][t] << endl;
	}
	
	if (stat.STKDV_type == 2)
	{
		outputFile << "M " << stat.M << endl;
		outputFile << "N " << stat.N << endl;

		for (int b_s_index = 0; b_s_index < stat.M; b_s_index++)
			for (int b_t_index = 0; b_t_index < stat.N; b_t_index++)
				for (int t = 0; t < stat.n_t; t++)
					for (int y = 0; y < stat.n_y; y++)
						for (int x = 0; x < stat.n_x; x++)
							outputFile << stat.out_multiple_tensors[b_s_index][b_t_index][x][y][t] << endl;
	}

	outputFile.close();
}