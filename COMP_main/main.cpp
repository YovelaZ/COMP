#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <tuple>
#include <cmath>
#include <string>
#include <cstdlib>

struct DataPoint {
    double x, y, t;
};

struct SampledData {
    double x;
    double y;
    double t;
    int weight;
};

struct statistics
{
    char* dataFileName;
    char* outputFileName;
    double b_s;
    double b_t;
    int kernel_type;
    double e;
};

int main(int argc, char**argv)
{
    statistics stat;
    stat.dataFileName = argv[1];
    stat.outputFileName = argv[2];
    std::stringstream ss(argv[3]);
    ss >> stat.b_s;

    std::stringstream ss2(argv[4]);
    ss2 >> stat.b_t;
    std::stringstream ss3(argv[5]);
    ss3 >> stat.kernel_type;
    std::stringstream ss4(argv[6]);
    ss4 >> stat.e;

    std::ifstream file(stat.dataFileName);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << stat.dataFileName << std::endl;
        return 1;
    }
    
    std::string line;
    int total_num = 0;
    double S_bandwidth1, T_bandwidth1;

    // Read the first line to get the total number of data points
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> total_num >> S_bandwidth1 >> T_bandwidth1;
    }

    std::vector<DataPoint> data;
    DataPoint dp;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> dp.x >> dp.y >> dp.t;
        data.push_back(dp);
    }
    file.close();

    // Calculate mean and standard deviation for S and T
    double sum_S = 0, sum_t = 0;
    double mean_S, mean_t;
    double sd_S = 0, sd_t = 0;

    // Calculate means
    for (int i = 0; i < total_num; i++) {
        sum_S += data[i].x;  
        sum_S += data[i].y;  
        sum_t += data[i].t;
    }
    mean_S = sum_S / 2 / total_num;
    mean_t = sum_t / total_num;

    // Calculate standard deviations
    for (int i = 0; i < total_num; i++) {
        sd_S += (data[i].x - mean_S) * (data[i].x - mean_S) / (2 * total_num - 1);
        sd_S += (data[i].y - mean_S) * (data[i].y - mean_S) / (2 * total_num - 1);
        sd_t += (data[i].t - mean_t) * (data[i].t - mean_t) / (total_num - 1);
    }
    sd_S = sqrt(sd_S);
    sd_t = sqrt(sd_t);

    // Pre-compute bandwidths
    double S_bandwidth_pre = pow(total_num, -1.0 / 6.0) * sd_S;
    double T_bandwidth_pre = pow(total_num, -1.0 / 5.0) * sd_t;

    // Vector to store the sampled data
    std::vector<SampledData> sampled_data;

    // Iterate through each group and compute the kernel density estimate
    double T_bandwidth = T_bandwidth_pre * stat.b_t;
    double S_bandwidth = S_bandwidth_pre * stat.b_s;

    // Bin sizes
    double x_bin_size = 0, y_bin_size = 0, t_bin_size = 0;

    if (stat.kernel_type == 1) // Epanechnikov
        x_bin_size = stat.e * S_bandwidth * 4 * std::sqrt(2) / 9;
        y_bin_size = stat.e * S_bandwidth * 4 * std::sqrt(2) / 9;
        t_bin_size = stat.e * T_bandwidth * 8 / 9;

    if (stat.kernel_type == 0) // Triangular
        x_bin_size = stat.e * S_bandwidth * std::sqrt(2) / 2;
        y_bin_size = stat.e * S_bandwidth * std::sqrt(2) / 2;
        t_bin_size = stat.e * T_bandwidth;

    if (stat.kernel_type == 2) // Quartic
        x_bin_size = stat.e * S_bandwidth * 16 * std::sqrt(6) / 75;
        y_bin_size = stat.e * S_bandwidth * 16 * std::sqrt(6) / 75;
        t_bin_size = stat.e * T_bandwidth * 32 * std::sqrt(3) / 75;

    // Group the data points by their binned values (x, y, t)
    std::map<std::tuple<int, int, int>, std::vector<DataPoint>> grouped;
    for (const auto& point : data) {
        int x_bin = static_cast<int>(point.x / x_bin_size);
        int y_bin = static_cast<int>(point.y / y_bin_size);
        int t_bin = static_cast<int>(point.t / t_bin_size);
        grouped[std::make_tuple(x_bin, y_bin, t_bin)].push_back(point);
    }

    // Process each group
    for (const auto& group : grouped) {
        auto name = group.first;  // Get the bin indices (x, y, t)
        int weight = group.second.size();  // Number of points in this group

        // Compute the bin center values
        double x_center = (std::get<0>(name) + 0.5) * x_bin_size;
        double y_center = (std::get<1>(name) + 0.5) * y_bin_size;
        double t_center = (std::get<2>(name) + 0.5) * t_bin_size;

        // Add the computed center and weight to the sampled data
        sampled_data.push_back({x_center, y_center, t_center, weight});
    }

    // Output results to a file
    std::ofstream outfile(stat.outputFileName);

    // Output the number of sampled data points and the bandwidths
    outfile << sampled_data.size() << " " << S_bandwidth << " " << T_bandwidth << std::endl;

    // Output the sampled data points (x_center, y_center, t_center, weight)
    for (const auto& data_point : sampled_data) {
        outfile << data_point.x << " "  
                << data_point.y << " "  
                << data_point.t << " "  
                << data_point.weight << std::endl;
    }

    outfile.close();
    sampled_data.clear();

    return 0;
}
