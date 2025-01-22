#include "alg_visual.h"

void visual_Algorithm(statistics& stat)
{
	double run_time;
	//initialization
	if (stat.STKDV_type == 0) //on-the-fly timestamps
		init_otf(stat);

	if (stat.STKDV_type == 1) //known timestamps
		init_kf(stat);

	if (stat.STKDV_type == 2) //bandwidth tuning
		init_bt(stat);

	//Visualization
	auto start_s = chrono::high_resolution_clock::now();

	if (stat.STKDV_type == 0) //on-the-fly timestamps
	{
		if (stat.method == 0) //SCAN method
			SS_otf(stat);
		if (stat.method == 1) //SWS method
			SWS_otf(stat);
		if (stat.method == 2) //PREFIX_single method
			PREFIX_single(stat);
	}
		
	if (stat.STKDV_type == 1) //known timestamps
	{
		if (stat.method == 0) //Used for checking the correctness of the results
		{
			init_check_multiple_time(stat);
			SS_otf(stat);
		}
		
		if (stat.method == 1) //SWS method
			SWS_kt(stat);

		if (stat.method == 2) //PREFIX_multiple method
			PREFIX_multiple(stat);
	}
	if (stat.STKDV_type == 2) //bandwidth tuning
	{
		if (stat.method == 0) //Used for checking the correctness of the results
			SS_bt(stat);

		if (stat.method == 1) //SWS method
			SWS_bt(stat);

		if (stat.method == 2) //PREFIX_tuning method
			PREFIX_tuning(stat);
	}

	auto end_s = chrono::high_resolution_clock::now();
	run_time = (chrono::duration_cast<chrono::nanoseconds>(end_s - start_s).count()) / 1000000000.0;
	cout << "method " << stat.method << ":" << run_time << endl;
    stat.runtime = run_time;
}