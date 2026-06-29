#include "../include/vector.hpp"
#include "../include/parser.hpp"
#include "../include/linkedqueue.hpp"
#include "../include/types.hpp"
#include "../include/scanner.hpp"
#include <iostream>
#include <chrono>
#include <filesystem>
namespace fs = std::filesystem;

int main() {
    auto start_io = std::chrono::high_resolution_clock::now();
    sc::vector<Pixel> image_pixels;
    std::string input{""};
    std::getline(std::cin, input);
    fs::path file(input);
    read_image(file, image_pixels);
    auto end_io = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_io = end_io - start_io;
    std::cout << "time de Leitura (I/O): " << time_io.count() << " ms\n";

    auto start_gray = std::chrono::high_resolution_clock::now();
    //code 
    convert_to_gray(); // falta os argumentos
    auto end_gray = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_gray = end_gray - start_gray;
    std::cout << "Grayscale Conversion Time: " << time_gray.count() << " ms\n";

    auto start_otsu = std::chrono::high_resolution_clock::now();
    //code here
    auto end_otsu = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_otsu = end_otsu - start_otsu;
    std::cout << "Otsu Threshold Calculation Time: " << time_otsu.count() << " ms\n";

    auto start_bin = std::chrono::high_resolution_clock::now();
    //code here
    auto end_bin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_bin = end_bin - start_bin;
    std::cout << "Binarization Time: " << time_bin.count() << " ms\n";

    std::cout << "MEMORY METRICS" << std::endl;
    std::cout << "Total relocations (resizes): " << image_pixels.get_resize_count() << "\n";
}