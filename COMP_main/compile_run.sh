### Compile our code ###
g++ -std=c++11 main.cpp -o main

##Input parameters to our C++ implementation###
# stat.dataFileName = argv[1]; //The input file name
# stat.outputFileName = argv[2]; //The output file name
# std::stringstream ss(argv[3]);
# ss >> stat.b_s; //control the bandwidth of spatial kernel
# std::stringstream ss2(argv[4]);
# ss2 >> stat.b_t; //control the bandwidth of temporal kernel
# std::stringstream ss3(argv[5]);
# ss3 >> stat.kernel_type; //0: Triangular kernel, 1: Epanechnikov kernel, 2: Quartic kernel
# std::stringstream ss4(argv[6]);
# ss4 >> stat.e; //control the error of COMP

dir="./data"
dataset="example_data"
result_dir="./Result"
b_s=1
b_t=1
kernel_type=1
e=0.05

# Echo the command for debugging purposes
echo "./main $dir/$dataset $result_dir/$dataset"_e$e"_bs$b_s"_bt$b_t"_K$kernel_type $b_s $b_t $kernel_type $e"
# Execute the command
./main $dir/$dataset $result_dir/$dataset"_e$e"_bs$b_s"_bt$b_t"_K$kernel_type $b_s $b_t $kernel_type $e
