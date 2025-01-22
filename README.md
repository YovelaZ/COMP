# Code Descriptions
This code is the implementation of our paper "A Fast and Accurate Block Compression Solution for Spatiotemporal Kernel Density Visualization", which is submitted to KDD 2025. Due to space limitations of the Github repository, we do not upload the datasets into this link. However, all datasets are the open data, which can be downloaded from the references in Table Ⅲ(see the column "Ref."). We have also attached ["example_data"](example_data) to show the format of the input data file. The data is stored in the following format: the first row, the data size (n), spatial bandwidth, and temporal bandwidth. Starting from the second row, each sample is recorded with columns for the x-coordinate (in meters), y-coordinate (in meters) and timestamp (in days).

# How to Call Our COMP Code?
First, use the code in the [COMP_main](COMP_main) folder for data compression, selecting the appropriate method based on the desired kernel function.
```
g++ -std=c++11 main.cpp -o main
```

After the compilation, you can run our COMP algorithm by executing the compiled C++ code with the correct parameters. This will generate the appropriate data results for both [COMP_PREFIX](COMP_PREFIX_main) and [COMP_SWS](COMP_SWS_main). The required parameters are shown in lines 35-45 of the C++ code snippet in [COMP_main/main.cpp].(In this section, the spatial and temporal bandwidths in the required data ["example_data"](example_data) can be replaced with null values.)
```
stat.dataFileName = argv[1]; //The input file name
stat.outputFileName = argv[2]; //The output file name
std::stringstream ss(argv[3]);
ss >> stat.b_s; //control the bandwidth of spatial kernel
std::stringstream ss2(argv[4]);
ss2 >> stat.b_t; //control the bandwidth of temporal kernel
std::stringstream ss3(argv[5]);
ss3 >> stat.kernel_type; //0: Triangular kernel, 1: Epanechnikov kernel, 2: Quartic kernel
std::stringstream ss4(argv[6]);
ss4 >> stat.e; //control the error of COMP
```
After successfully running the COMP code, you will obtain the compressed data suitable for [COMP_PREFIX](COMP_PREFIX_main) and [COMP_SWS](COMP_SWS_main). 

## Example 1: COMP with Epanechnikov kernels
Here, we illustrate how to call our COMP to generate the compressed data in the default region with the error 0.05 in the New York traffic accident dataset (with the input file name "New_York") using the Epanechnikov function as spatial and temporal kernels. The shell script code is shown as follows.
```
dir="./data"
dataset="New_York"
result_dir="./Result"
b_s=1
b_t=1
kernel_type=1
e=0.05

./main $dir/$dataset $result_dir/$dataset"_e$e"_bs$b_s"_bt$b_t"_K$kernel_type $b_s $b_t $kernel_type $e
```
## Example 2: COMP with Triangular kernels
Here, we illustrate how to call our COMP to generate the compressed data in the default region with the error 0.05 in the New York traffic accident dataset (with the input file name "New_York") using the Triangular function as spatial and temporal kernels. The shell script code is shown as follows.
```
dir="./data"
dataset="New_York"
result_dir="./Result"
b_s=1
b_t=1
kernel_type=0
e=0.05

./main $dir/$dataset $result_dir/$dataset"_e$e"_bs$b_s"_bt$b_t"_K$kernel_type $b_s $b_t $kernel_type $e
```
## Example 3: COMP with Quartic kernels
Here, we illustrate how to call our COMP to generate the compressed data in the default region with the error 0.05 in the New York traffic accident dataset (with the input file name "New_York") using the Quartic function as spatial and temporal kernels. The shell script code is shown as follows.
```
dir="./data"
dataset="New_York"
result_dir="./Result"
b_s=1
b_t=1
kernel_type=2
e=0.05

./main $dir/$dataset $result_dir/$dataset"_e$e"_bs$b_s"_bt$b_t"_K$kernel_type $b_s $b_t $kernel_type $e
```
# How to Run Our STKDV Data Calculation Code?(SWS, COMP_SWS, PREFIX and COMP_PREFIX)
## PREFIX and COMP_PREFIX
In order to compile our [PREFIX](PREFIX_main) and [COMP_PREFIX](COMP_PREFIX_main) C++ code, you need to write the following shell scripts in the ".sh file".
```
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
```
After you have successfully compiled our C++ code, you can run our C++ code by inputting the correct parameters, which are shown in the following C++ code fragment (lines 38 to 73 in ["PREFIX_main/init_visual.cpp"](PREFIX_main/init_visual.cpp) and ["COMP_PREFIX_main/init_visual.cpp"](COMP_PREFIX_main/init_visual.cpp)).

```
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
```
Here, we provide the description of each input parameter.

"stat.dataFileName" represents the input data file name.

"stat.outputFileName" represents the output result file name.

"stat.STKDV_type" denotes different problem settings (0: STKDV with on-the-fly timestamps, 1: STKDV with known timestamps, and 2: Bandwidth tuning).

"stat.method" denotes different methods (1: SWS, 2: PREFIX).

"stat.n_x" represents the resolution size in the x-axis.

"stat.n_y" represents the resolution size in the y-axis.

"stat.n_t" represents the resolution size in the time-axis (or t-axis).

"stat.k_type_s" represents the spatial kernel function (1: Epanechnikov kernel, 2: Quartic kernel, 3: Uniform kernel).

"stat.k_type_t" represents the temporal kernel function (1: Epanechnikov kernel, 2: Quartic kernel, 3: Uniform kernel).

"stat.b_s_ratio" controls the bandwidth of spatial kernel.

"stat.b_t_ratio" controls the bandwidth of temporal kernel.

"stat.is_default_region" denotes whether we use the default region (i.e., using the minimum bounding rectangle that covers all data points) or select the region (1 (True): default region, 0 (False): Need to select the region). In this paper, we mainly use the default region for testing.

"stat.x_L", "stat.x_U", "stat.y_L", and "stat.y_U" denote the smallest x-coordinate, the largest x-coordinate, the smallest y-coordinate, and the largest y-coordinate of the geographical region that we would like to visualize. These parameters are used only if we set 0 in "stat.is_default_region".

"stat.t_L" and "stat.t_U" denote the smallest t-coordinate and the largest t-coordinate that we would like to consider. These parameters are used only if we set 0 in "stat.is_default_region".

"stat.M" denotes the number of spatial bandwidths. This parameter is used when we consider the bandwidth tuning problem (i.e., stat.STKDV_type = 2).

"stat.N" denotes the number of temporal bandwidths. This parameter is used when we consider the bandwidth tuning problem (i.e., stat.STKDV_type = 2).

### Example 1: PREFIX
In this paper's experiments, we use the STKDV with Known Timestamps setup. To generate STKDV in the default region with a resolution of 1280 × 960 and 32 known timestamps from the New York traffic accident dataset (input file: 'New_York'), using the Epanechnikov kernel for both spatial and temporal components, set STKDV_type = 1.
```
dir="./"
dataset="New_York"
result_dir="./Results/"
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

./main $dir$dataset"/"$dataset $result_dir$dataset"_X"$n_x"_Y"$n_y"_T"$n_t"_STKDV"$STKDV_type"_M"$method"_bs"$b_s_ratio"_bt"$b_t_ratio $STKDV_type $method $n_x $n_y $n_t $k_type_s $k_type_t $b_s_ratio $b_t_ratio $is_default_region
```

### Example 2: COMP_PREFIX
Here, we demonstrate STKDV using COMP-processed data, in the default region with a resolution of 1280 × 960 and 32 known timestamps from the New York traffic accident dataset (input file: 'New_York'). The Epanechnikov kernel is used for both spatial and temporal components, with STKDV_type set to 1.
```
dir="./COMP_DATA"
dataset="New_York"
result_dir="./Results/"
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
e=0.05
bs=1
bt=1

./main $dir"/"$dataset"_e"$e"_bs"$bs"_bt"$bt"_K"$k_type_s $result_dir$dataset"_e"$e"_X"$n_x"_Y"$n_y"_T"$n_t"_STKDV"$STKDV_type"_M"$method"_bs"$bs"_bt"$bt $STKDV_type $method $n_x $n_y $n_t $k_type_s $k_type_t $b_s_ratio $b_t_ratio $is_default_region
```

## SWS and COMP_SWS
In order to compile our [SWS](SWS_main) and [COMP_SWS](COMP_SWS_main) C++ code, you need to write the following shell scripts in the ".sh file".
```
g++ -c init_visual.cpp -w -o init_visual.o -std=c++11
g++ -c SS.cpp -w -o SS.o -std=c++11
g++ -c Tree.cpp -w -o Tree.o -std=c++11
g++ -c kd_Tree.cpp -w -o kd_Tree.o -std=c++11
g++ -c ball_Tree.cpp -w -o ball_Tree.o -std=c++11
g++ -c SWS.cpp -w -o SWS.o -std=c++11
g++ -c alg_visual.cpp -w -o alg_visual.o -std=c++11
g++ main.cpp -O3 -o main init_visual.o SS.o Tree.o kd_Tree.o ball_Tree.o SWS.o alg_visual.o
```
After you have successfully compiled our C++ code, you can run our C++ code by inputting the correct parameters, which are shown in the following C++ code fragment (lines 38 to 68 in ["SWS_main/init_visual.cpp"](PREFIX_main/init_visual.cpp) and ["COMP_SWS_main/init_visual.cpp"](COMP_PREFIX_main/init_visual.cpp)).

```
	stat.dataFileName = argv[1];
	stat.outputFileName = argv[2];
	stat.method = atoi(argv[3]);
	stat.n_x = atoi(argv[4]);
	stat.n_y = atoi(argv[5]);
	stat.n_t = atoi(argv[6]);
	stat.k_type_s = atoi(argv[7]);
	stat.k_type_t = atoi(argv[8]);
	stat.gamma_ratio_s = atof(argv[9]);
	stat.gamma_ratio_t = atof(argv[10]);
	stat.b = atof(argv[11]);
	stat.specify_region = atoi(argv[12]);
	stat.is_progressive = atoi(argv[13]);

	if (stat.specify_region == true) //We have specified the region
	{
		stat.x_L = atof(argv[14]);
		stat.x_U = atof(argv[15]);
		stat.y_L = atof(argv[16]);
		stat.y_U = atof(argv[17]);
		stat.t_L = atof(argv[18]);
		stat.t_U = atof(argv[19]);

		if (stat.is_progressive == true)
			stat.prev_level_resultfileName = argv[20];
	}
	else //Use the default region
	{
		if (stat.is_progressive == true)
			stat.prev_level_resultfileName = argv[14];
	}
```
Here, we provide the description of each input parameter.

"stat.dataFileName" represents the input data file name.

"stat.outputFileName" represents the output result file name.

"stat.method" denotes different methods (4: SWS or COMP_SWS).

"stat.n_x" represents the resolution size in the x-axis.

"stat.n_y" represents the resolution size in the y-axis.

"stat.n_t" represents the resolution size in the time-axis (or t-axis).

"stat.k_type_s" represents the spatial kernel function (1: Epanechnikov kernel, 2: Quartic kernel, 3: Uniform kernel).

"stat.k_type_t" represents the temporal kernel function (1: Epanechnikov kernel, 2: Quartic kernel, 3: Uniform kernel).

"stat.gamma_ratio_s" controls the bandwidth of spatial kernel.(gamma=1/bandwidth)

"stat.gamma_ratio_t" controls the bandwidth of temporal kernel.(gamma=1/bandwidth)

### Example 1: SWS
In this paper's experiments, we use the STKDV with Known Timestamps setup. To generate STKDV in the default region with a resolution of 1280 × 960 and 32 known timestamps from the New York traffic accident dataset (input file: 'New_York'), using the Epanechnikov kernel for both spatial and temporal components, set STKDV_type = 1.
```
dir="./"
dataset="New_York"
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
#4: SWS

./main $dir$dataset"/"$dataset $result_dir$dataset"_X"$n_x"_Y"$n_y"_Z"$n_z"_M"$method"_gs_"$gamma_ratio_s"_gt_"$gamma_ratio_t"_K"$k_type_s $method $n_x $n_y $n_z $k_type_s $k_type_t $gamma_ratio_s $gamma_ratio_t $b $specify_region $is_progressive
```

### Example 2: COMP_SWS
Here, we demonstrate STKDV using COMP-processed data, in the default region with a resolution of 1280 × 960 and 32 known timestamps from the New York traffic accident dataset (input file: 'New_York'). The Epanechnikov kernel is used for both spatial and temporal components, with STKDV_type set to 1.
```
dir="./COMP_DATA/"
dataset="New_York"
result_dir="./Results/"

e=0.05
bs=1
bt=1
n_x=1280
n_y=960
n_z=32

k_type_s=1
k_type_t=1
gamma_ratio_s=1
gamma_ratio_t=1
is_progressive=0
specify_region=0
b=1

method=4
#4: SWS

./main $dir$dataset"_e"$e"_bs"$bs"_bt"$bt"_K"$k_type_s $result_dir$dataset"_e"$e"_X"$n_x"_Y"$n_y"_Z"$n_z"_M"$method"_bs"$bs"_bt"$bt"_K"$k_type_t $method $n_x $n_y $n_z $k_type_s $k_type_t $gamma_ratio_s $gamma_ratio_t $b $specify_region $is_progressive
```
