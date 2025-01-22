#Compile the program
g++ -c init_visual.cpp -w -o init_visual.o -std=c++11
g++ -c SS.cpp -w -o SS.o -std=c++11
g++ -c Tree.cpp -w -o Tree.o -std=c++11
g++ -c kd_Tree.cpp -w -o kd_Tree.o -std=c++11
g++ -c ball_Tree.cpp -w -o ball_Tree.o -std=c++11
g++ -c SWS.cpp -w -o SWS.o -std=c++11
g++ -c alg_visual.cpp -w -o alg_visual.o -std=c++11
g++ main.cpp -O3 -o main init_visual.o SS.o Tree.o kd_Tree.o ball_Tree.o SWS.o alg_visual.o

#Parameters in our code
#stat.dataFileName = argv[1];
#The data file name of the location datasets

#stat.outputFileName = argv[2]; 
#The STKDV result

#stat.method = atoi(argv[3]); 
#method = 0: SCAN 
#method = 1: RQS_{kd} 
#method = 2: RQS_{ball} 
#method = 4: SWS 
#method = 5: parallel version of SCAN
#method = 6: parallel version of RQS_{kd}
#method = 7: parallel version of RQS_{ball}

#stat.n_x = atoi(argv[4]);
#number of voxels in x-axis

#stat.n_y = atoi(argv[5]);
#number of voxels in y-axis

#stat.n_t = atoi(argv[6]);
#number of voxels in t-axis

#stat.k_type_s = atoi(argv[7]);
#spatial kernel type
#k_type_s = 0: triangular kernel
#k_type_s = 1: Epanechnikov kernel
#k_type_s = 2: quartic kernel

#stat.k_type_t = atoi(argv[8]);
#temporal kernel type
#k_type_t = 0: triangular kernel
#k_type_t = 1: Epanechnikov kernel
#k_type_t = 2: quartic kernel

#stat.gamma_ratio_s = atof(argv[9]);
#Used to vary the parameter \gamma_s (i.e., Figures 13, 17a and 17c) in our paper (the default value is 1)

#stat.gamma_ratio_t = atof(argv[10]);
#Used to vary the parameter \gamma_s (i.e., Figures 14, 17b and 17d) in our paper (the default value is 1)

#stat.b = atof(argv[11]);
#The constant b (set it to be 1) in the paper "E. Pan and P. Bailis "Scalable Kernel Density Classification via Threshold-Based Pruning", SIGMOD2017"

#stat.specify_region = atoi(argv[12]);
#specify_region = 0 means we use the minimum bounding rectangle for the data points in the location dataset to represent the region.
#specify_region = 1 means we need to specify the visualized region.

#stat.is_progressive = atoi(argv[13]);
#is_progressive = 0 means we do not use the progressive visualization framework
#is_progressive = 1 means we use the progressive visualization framework

#if (stat.specify_region == true) //We have specified the region
#{
#	//Specify the x_L, x_U, y_L, y_U, t_L and t_U
#	stat.x_L = atof(argv[14]); 
#	stat.x_U = atof(argv[15]);
#	stat.y_L = atof(argv[16]);
#	stat.y_U = atof(argv[17]);
#	stat.t_L = atof(argv[18]);
#	stat.t_U = atof(argv[19]);
#
#	if (stat.is_progressive == true)
#		stat.prev_level_resultfileName = argv[20]; //Specify the prefix of the result file
#}
#else //Use the default region
#{
#	if (stat.is_progressive == true)
#		stat.prev_level_resultfileName = argv[14]; //Specify the prefix of the result file
#}

#One example for running our code

dir="./"
dataset="New_York_Crashes"
result_dir="./Results/"

k_type_s=1
k_type_t=1
gamma_ratio_s=1
gamma_ratio_t=1
is_progressive=0
specify_region=0
b=1
n_x=1280
n_y=960
n_z=32

method=4
#0: SCAN方法
#1: RQS方法（使用kd-tree）
#2: RQS方法（使用ball-tree）
#4: SWS方法
#8: SWS方法（使用kd-tree）

echo ./main $dir$dataset"/"$dataset $result_dir$dataset"_X"$n_x"_Y"$n_y"_Z"$n_z"_M"$method"_gs_"$gamma_ratio_s"_gt_"$gamma_ratio_t"_K"$k_type_s $method $n_x $n_y $n_z $k_type_s $k_type_t $gamma_ratio_s $gamma_ratio_t $b $specify_region $is_progressive
./main $dir$dataset"/"$dataset $result_dir$dataset"_X"$n_x"_Y"$n_y"_Z"$n_z"_M"$method"_gs_"$gamma_ratio_s"_gt_"$gamma_ratio_t"_K"$k_type_s $method $n_x $n_y $n_z $k_type_s $k_type_t $gamma_ratio_s $gamma_ratio_t $b $specify_region $is_progressive
