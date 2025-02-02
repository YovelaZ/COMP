#include "alg_visual.h"
#include "init_visual.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char**argv)
{
	statistics stat;
	init_visual(argc, argv, stat);
	visual_Algorithm(stat);
	save_result_to_file(stat);
	std::string filename = "./Results/time.txt"; 
    	std::string new_line = argv[2] + std::string(" needs ") + std::to_string(stat.runtime) + " seconds.";

    	std::ifstream file(filename, std::ios::in | std::ios::binary);
    	if (!file.is_open()) {
        	std::cerr << "Failed to open file for reading." << std::endl;
        	return 1;
    	}

    	std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    	file.close(); 

    	if (buffer.find(new_line) == std::string::npos) {  
        	if (!buffer.empty() && buffer.back() == '\n') {
            		buffer.pop_back();
        	}

        	std::ofstream outfile(filename, std::ios::out | std::ios::trunc);
        	if (!outfile.is_open()) {
            		std::cerr << "Failed to open file for writing." << std::endl;
            		return 1;
        	}
        	outfile << buffer << "\n" << new_line;

        	outfile.close();
    	} else {
        	std::cout << "New line already exists in the file." << std::endl;
    	}
}
