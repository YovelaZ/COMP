# Code Descriptions
This code is the implementation of our paper "Large-scale Spatiotemporal Kernel Density Visualization", which is submitted to ICDE 2025. Due to space limitations of the Github repository, we do not upload the datasets into this link. However, all datasets are the open data, which can be downloaded from the references in Table III (see the column "Ref."). We have also attached "[example_data_file](example_data_file)" to show the format of the input data file.

The file "[otf_visual.cpp](otf_visual.cpp)" shows the implementation of the methods, SWS (lines 37 to 63) and PREFIX<sub>single</sub> (lines 90 to 124), for generating STKDV (see Problem 1) with on-the-fly timestamps.

The file "[kt_visual.cpp](kt_visual.cpp)" shows the implementation of the methods, SWS (lines 136 to 145) and PREFIX<sub>multiple</sub> (lines 47 to 134), for generating STKDV (see Problem 1) with T known timestamps.

The file "[bt_visual.cpp](bt_visual.cpp)" shows the implementation of the methods, SWS (lines 216 to 239) and PREFIX<sub>tuning</sub> (lines 118 to 214), for bandwidth tuning (see Problem 2).

# How to Call Our Code?
In order to compile our C++ code, you need to write the following shell scripts in the ".sh file".
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

After you have successfully compiled our C++ code, you can run our C++ code by inputting the correct parameters, which are shown in the following C++ code fragment (lines 38 to 73 in "[init_visual.cpp](init_visual.cpp)").

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

## Example 1: STKDV with On-the-fly Timestamps
Here, we illustrate how to use call our PREFIX<sub>single</sub> to generate STKDV in the default region with the resolution size 1280 $\times$ 960 and 32 on-the-fly timestamps in the New York traffic accident dataset (with the input file name "New_York") using the Epanechnikov function as spatial and temporal kernels. The shell script code is shown as follows.

```
dataFileName="New_York"
outputFileName="./Results/New_York_STKDV_0_Method_2"
STKDV_type=0
method=2
n_x=1280
n_y=960
n_t=32
k_type_s=1
k_type_t=1
b_s_ratio=1
b_t_ratio=1
is_default_region=1
./main $dataFileName $outputFileName $STKDV_type $method $n_x $n_y $n_t $k_type_s $k_type_t $b_s_ratio $b_t_ratio $is_default_region
```

## Example 2: STKDV with Known Timestamps
In order to generate STKDV in the default region with the resolution size 1280 $\times$ 960 and 32 known timestamps in the New York traffic accident dataset (with the input file name "New_York") using the Epanechnikov function as spatial and temporal kernels, we can simply set STKDV_type = 1. 
```
dataFileName="New_York"
outputFileName="./Results/New_York_STKDV_1_Method_2"
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
./main $dataFileName $outputFileName $STKDV_type $method $n_x $n_y $n_t $k_type_s $k_type_t $b_s_ratio $b_t_ratio $is_default_region
```

## Example 3: Bandwidth Tuning
Here, we consider an example to support bandwidth tuning in the New York traffic accident dataset (with the input file name "New_York") with three spatial bandwidths and five temporal bandwidths, where the resolution size and the number of timestamps of each STKDV are fixed to be 1280 $\times$ 960 and 32, respectively. Moreover, we also adopt the Epanechnikov function as the spatial and temporal kernels. The shell script code is shown as follows. 
```
dataFileName="New_York"
outputFileName="./Results/New_York_STKDV_2_Method_2"
STKDV_type=2
method=2
n_x=1280
n_y=960
n_t=32
k_type_s=1
k_type_t=1
b_s_ratio=1
b_t_ratio=1
is_default_region=1
M=3
N=5
./main $dataFileName $outputFileName $STKDV_type $method $n_x $n_y $n_t $k_type_s $k_type_t $b_s_ratio $b_t_ratio $is_default_region $M $N
```

# PREFIX<sub>tuning</sub>: A Holistic Solution for Generating STKDV and Supporting Bandwidth Tuning
In order to ease the implementation of our PREFIX solution for further development, we would like to point out that PREFIX<sub>tuning</sub> can act as a holistic solution for supporting all problem settings, including (i) STKDV with on-the-fly timestamps, (2) STKDV with known timestamps, and (3) bandwidth tuning, without incurring any additional time complexity and additional space complexity. Consider the STKDV problem with $T$ known timestamps (see Problem 1). This problem is the special case of the bandwidth tuning problem with one spatial bandwidth and one temporal bandwidth (i.e., $M=1$ and $N=1$ in Problem 2). Therefore, once we set $M=1$ and $N=1$ for the PREFIX<sub>tuning</sub> method, it returns the result of STKDV with $T$ known timestamps. The time complexity and space complexity of PREFIX<sub>tuning</sub> in this case are also $O(XYT+Yn)$ and $O(XYT+n)$, respectively (see Table II), which are the same as the ones of PREFIX<sub>multiple</sub>. Similarly, the STKDV problem with one (on-the-fly) timestamp is also the special case of the bandwidth tuning problem with one spatial bandwidth ($M=1$), one temporal bandwidth ($N=1$), and one timestamp ($T=1$). Hence, the time complexity and space complexity of PREFIX<sub>tuning</sub> in this case are also $O(Y(X+n))$ and $O(XY+n)$, respectively (see Table II), which are the same as the ones of PREFIX<sub>single</sub>.

<!--- The shell script file "[compile_run.sh](compile_run.sh)" includes the details for compiling and running our C++ code.--->


<!--- # PREFIX (Technical Report)
Due to space limitations of PVLDB, we do not provide a detailed proof of Lemma 1 in the paper. In this Github link, we also include the technical report ([PREFIX_TR.pdf](PREFIX_TR.pdf)), which shows the proof in Section 7. Since the letters and figures are deformed when you view the pdf file in the anonymized Github link, you can first download this file and then read it in your local computer.--->
