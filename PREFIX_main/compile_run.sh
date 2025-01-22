###Compile our code###
g++ -c init_visual.cpp -o init_visual.o
g++ -c Euclid_Bound.cpp -o Euclid_Bound.o
g++ -c SS.cpp -o SS.o
g++ -c SWS.cpp -o SWS.o
g++ -c bucket.cpp -o bucket.o
g++ -c otf_visual.cpp -o otf_visual.o
g++ -c kt_visual.cpp -o kt_visual.o
g++ -c bt_visual.cpp -o bt_visual.o
g++ -c alg_visual.cpp -o alg_visual.o
g++ main.cpp -O3 -o main init_visual.o Euclid_Bound.o SS.o SWS.o bucket.o otf_visual.o kt_visual.o bt_visual.o alg_visual.o
# exit #Remove this "exit" if you would like to run our code.

###Input parameters to our C++ implementation###
#stat.dataFileName = argv[1]; //The input file name
#stat.outputFileName = argv[2]; //The output file name
#stat.STKDV_type = atoi(argv[3]); //0: STKDV with on-the-fly timestamps, 1: STKDV with known timestamps, 2: Bandwidth tuning
#stat.method = atoi(argv[4]); //1: SWS, 2: PREFIX
#stat.n_x = atoi(argv[5]); //resolution size in the x-axis
#stat.n_y = atoi(argv[6]); //resolution size in the y-axis
#stat.n_t = atoi(argv[7]); //resolution size in the time-axis (or t-axis)
#stat.k_type_s = atoi(argv[8]); //1: Epanechnikov kernel, 2: Quartic kernel, 3: Uniform kernel
#stat.k_type_t = atoi(argv[9]); //1: Epanechnikov kernel, 2: Quartic kernel, 3: Uniform kernel
#stat.b_s_ratio = atof(argv[10]); //control the bandwidth of spatial kernel
#stat.b_t_ratio = atof(argv[11]); //control the bandwidth of temporal kernel
#stat.is_default_region = (bool)atoi(argv[12]); //1 (True): The minimum bounding rectangle that covers all data points 0 (False): Need to select the region

#if (stat.is_default_region == false)
#{
#	stat.x_L = atof(argv[13]); //the smallest x-coordinate
#	stat.x_U = atof(argv[14]); //the largest x-coordinate
#	stat.y_L = atof(argv[15]); //the smallest y-coordinate
#	stat.y_U = atof(argv[16]); //the largest y-coordinate
#	stat.t_L = atof(argv[17]); //the smallest t-coordinate
#	stat.t_U = atof(argv[18]); //the largest t-coordinate
#
#	if (stat.STKDV_type == 2) //bandwidth tuning
#	{
#		stat.M = atoi(argv[19]); //the number of spatial bandwidths
#		stat.N = atoi(argv[20]); //the number of temporal bandwidths
#	}
#}
#else
#{
#	if (stat.STKDV_type == 2) //bandwidth tuning
#	{
#		stat.M = atoi(argv[13]); //the number of spatial bandwidths
#		stat.N = atoi(argv[14]); //the number of temporal bandwidths
#	}
#}

###Example for calling our method, PREFIX, to support STKDV with the 1280 x 960 resolution size and 32 known timestamps (STKDV_type = 1) using the default region.###
#dataFileName="./Kansas_crime/Kansas_crime"
dir="./"
dataset="New_York_Crashes"
result_dir="./Results/"
#outputFileName="./Results/Kansas_crime_STKDV_1_Method_2_K1"
STKDV_type=1


method=2
n_x=1280
n_y=960
n_t=32
k_type_s=1
k_type_t=1
b_s_ratio=1
b_t_ratio=1
is_default_region=1
echo ./main $dir$dataset"/"$dataset $result_dir$dataset"_X"$n_x"_Y"$n_y"_T"$n_t"_STKDV"$STKDV_type"_M"$method"_bs"$b_s_ratio"_bt"$b_t_ratio $STKDV_type $method $n_x $n_y $n_t $k_type_s $k_type_t $b_s_ratio $b_t_ratio $is_default_region
./main $dir$dataset"/"$dataset $result_dir$dataset"_X"$n_x"_Y"$n_y"_T"$n_t"_STKDV"$STKDV_type"_M"$method"_bs"$b_s_ratio"_bt"$b_t_ratio $STKDV_type $method $n_x $n_y $n_t $k_type_s $k_type_t $b_s_ratio $b_t_ratio $is_default_region
