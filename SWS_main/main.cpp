#include "alg_visual.h"
#include "init_visual.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char**argv)
{
    auto start = std::chrono::high_resolution_clock::now();
	statistics stat;
	init_visual(argc, argv, stat);
	visual_Algorithm(stat);
	save_result_to_file(stat);
    // 程序结束的时间点
    auto end = std::chrono::high_resolution_clock::now();

    // 计算程序运行时间
    std::chrono::duration<double> duration = end - start;

    // 输出程序运行时间
    std::cout << "Total runtime: " << duration.count() << " seconds" << std::endl;

    std::string filename = "./Results/time.txt"; // 替换为你的文件名
    std::string new_line = argv[2] + std::string(" needs ") + std::to_string(stat.runtime) + " seconds."; // 要添加的新行

    // 以读取模式打开文件
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return 1;
    }

    // 将整个文件内容读取到字符串缓冲区
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close(); // 关闭文件

    // 确保新内容不会重复写入
    if (buffer.find(new_line) == std::string::npos) {  // 检查新行是否已存在
        // 移除缓冲区末尾的多余换行符（如有），以避免写入时多余的空行
        if (!buffer.empty() && buffer.back() == '\n') {
            buffer.pop_back();
        }


        // 以写模式打开文件并截断内容
        std::ofstream outfile(filename, std::ios::out | std::ios::trunc);
        if (!outfile.is_open()) {
            std::cerr << "Failed to open file for writing." << std::endl;
            return 1;
        }

        // 写入原始内容和新行到文件中
        outfile << buffer << "\n" << new_line;

        // 关闭输出文件
        outfile.close();
    } else {
        std::cout << "New line already exists in the file." << std::endl;
    }

}
