#include "benchmark_helper_functions.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  std::string output_path;
  std::string image_path;
  bool use_input_image{false};

  try {
    benchmark::parse_arguments(argc, argv, output_path, image_path, use_input_image);
  } catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
  }

  std::ofstream csv(output_path);
  if (!csv.is_open()) {
    std::cerr << "Erro ao abrir arquivo de saida: " << output_path << '\n';
    return 1;
  }

  csv << std::fixed << std::setprecision(6);
  benchmark::write_csv_header(csv);

  std::cout << "Saida CSV: " << output_path << '\n';

  try {
    if (use_input_image) {
      benchmark::run_image_benchmark(csv, image_path);
    } else {
      benchmark::run_synthetic_benchmark(csv);
    }
  } catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
  }

  std::cout << "Benchmark concluído. CSV gerado em " << output_path << '\n';
  return 0;
}