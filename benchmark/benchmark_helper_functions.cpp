#include "benchmark_helper_functions.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace benchmark {

Pixel make_pixel(int x, int y) {
  Pixel pixel{};
  pixel.r = static_cast<std::uint8_t>((x * 31 + y * 17) % 256);
  pixel.g = static_cast<std::uint8_t>((x * 13 + y * 29 + 64) % 256);
  pixel.b = static_cast<std::uint8_t>((x * 7 + y * 3 + x * y) % 256);
  return pixel;
}

std::vector<Pixel> generate_source_image(int width, int height) {
  const std::size_t total_pixels = static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
  std::vector<Pixel> pixels;
  pixels.reserve(total_pixels);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      pixels.push_back(make_pixel(x, y));
    }
  }

  return pixels;
}

ImageData load_image_from_file(const std::string &path) {
  std::ifstream input(path);
  if (!input.is_open()) {
    throw std::runtime_error("Erro ao abrir: " + path);
  }

  std::string type;
  int width{0};
  int height{0};
  int max_value{0};

  input >> type >> width >> height >> max_value;
  if (type != "P3" && type != "P2") {
    throw std::runtime_error("Formato invalido: " + type);
  }

  ImageData image{};
  image.width = width;
  image.height = height;
  image.pixels.reserve(static_cast<std::size_t>(width) * static_cast<std::size_t>(height));

  for (int idx = 0; idx < width * height; ++idx) {
    int r{0};
    int g{0};
    int b{0};

    if (type == "P3") {
      input >> r >> g >> b;
    } else {
      input >> r;
      g = b = r;
    }

    Pixel pixel{};
    pixel.r = static_cast<std::uint8_t>(r);
    pixel.g = static_cast<std::uint8_t>(g);
    pixel.b = static_cast<std::uint8_t>(b);
    image.pixels.push_back(pixel);
  }

  return image;
}

uint8_t convert_to_gray(const Pixel &p) {
  const float gray = 0.299f * p.r + 0.587f * p.g + 0.114f * p.b;
  return static_cast<std::uint8_t>(gray);
}

uint8_t calculate_otsu_threshold(const std::array<std::uint32_t, 256> &histogram,
                                 std::size_t total_pixels) {
  double total_sum{0.0};
  for (std::size_t idx = 0; idx < histogram.size(); ++idx) {
    total_sum += static_cast<double>(idx) * static_cast<double>(histogram[idx]);
  }

  double background_sum{0.0};
  std::uint32_t weight_background{0};
  std::uint32_t weight_foreground{0};
  double max_variance{0.0};
  std::size_t best_threshold{0};

  for (std::size_t idx = 0; idx < histogram.size(); ++idx) {
    weight_background += histogram[idx];
    if (weight_background == 0) {
      continue;
    }

    weight_foreground = static_cast<std::uint32_t>(total_pixels) - weight_background;
    if (weight_foreground == 0) {
      break;
    }

    background_sum += static_cast<double>(idx) * static_cast<double>(histogram[idx]);

    const double mean_background = background_sum / static_cast<double>(weight_background);
    const double mean_foreground = (total_sum - background_sum) / static_cast<double>(weight_foreground);
    const double variance = static_cast<double>(weight_background) * static_cast<double>(weight_foreground) *
                            (mean_background - mean_foreground) * (mean_background - mean_foreground);

    if (variance > max_variance) {
      max_variance = variance;
      best_threshold = idx;
    }
  }

  return static_cast<uint8_t>(best_threshold);
}

void write_csv_header(std::ofstream &csv) {
  csv << "Total_Pixels,Largura,Altura,Tempo_NossoVetor_ms,Tempo_StdVector_ms,"
         "Tempo_Build_NossoVetor_ms,Tempo_Hist_NossoVetor_ms,Tempo_Otsu_NossoVetor_ms,"
         "Tempo_Binarizacao_NossoVetor_ms,Reallocacoes_NossoVetor,Threshold_NossoVetor,"
         "Checksum_NossoVetor,Tempo_Build_StdVector_ms,Tempo_Hist_StdVector_ms,"
         "Tempo_Otsu_StdVector_ms,Tempo_Binarizacao_StdVector_ms,Reallocacoes_StdVector,"
         "Threshold_StdVector,Checksum_StdVector\n";
}

void write_csv_row(std::ofstream &csv, const BenchmarkRow &row) {
  const std::size_t total_pixels = static_cast<std::size_t>(row.width) * static_cast<std::size_t>(row.height);

  csv << total_pixels << ','
      << row.width << ','
      << row.height << ','
      << row.custom.total_ms << ','
      << row.standard.total_ms << ','
      << row.custom.build_ms << ','
      << row.custom.histogram_ms << ','
      << row.custom.otsu_ms << ','
      << row.custom.binarization_ms << ','
      << row.custom.reallocations << ','
      << static_cast<unsigned int>(row.custom.threshold) << ','
      << row.custom.checksum << ','
      << row.standard.build_ms << ','
      << row.standard.histogram_ms << ','
      << row.standard.otsu_ms << ','
      << row.standard.binarization_ms << ','
      << row.standard.reallocations << ','
      << static_cast<unsigned int>(row.standard.threshold) << ','
      << row.standard.checksum << '\n';
}

bool parse_arguments(int argc, char **argv, std::string &output_path, std::string &image_path, bool &use_input_image) {
  output_path = "dados_benchmark.csv";
  use_input_image = false;
  image_path.clear();

  for (int idx = 1; idx < argc; ++idx) {
    const std::string argument = argv[idx];

    if (argument == "-i") {
      if (idx + 1 >= argc) {
        throw std::runtime_error("A flag -i exige um caminho de imagem.");
      }

      image_path = argv[++idx];
      use_input_image = true;
      continue;
    }

    if (argument == "-o") {
      if (idx + 1 >= argc) {
        throw std::runtime_error("A flag -o exige um caminho de saida.");
      }

      output_path = argv[++idx];
      continue;
    }

    if (!argument.empty() && argument[0] != '-') {
      output_path = argument;
    }
  }

  return true;
}

void run_synthetic_benchmark(std::ofstream &csv) {
  const std::array<int, 10> sizes{100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};

  std::cout << "Executando benchmark comparativo com imagens sinteticas de 100x100 ate 1000x1000...\n";

  for (int side : sizes) {
    const auto source = generate_source_image(side, side);

    const PhaseTimes custom = run_pipeline_for_container<sc::vector<Pixel>>(source);
    const PhaseTimes standard = run_pipeline_for_container<std::vector<Pixel>>(source);

    write_csv_row(csv, BenchmarkRow{side, side, custom, standard});

    std::cout << side << "x" << side << " -> "
              << "custom=" << custom.total_ms << " ms, "
              << "std=" << standard.total_ms << " ms, "
              << "reallocacoes_custom=" << custom.reallocations << ", "
              << "reallocacoes_std=" << standard.reallocations << '\n';
  }
}

void run_image_benchmark(std::ofstream &csv, const std::string &image_path) {
  const ImageData image = load_image_from_file(image_path);

  std::cout << "Executando benchmark com imagem externa: " << image_path << '\n';

  const PhaseTimes custom = run_pipeline_for_container<sc::vector<Pixel>>(image.pixels);
  const PhaseTimes standard = run_pipeline_for_container<std::vector<Pixel>>(image.pixels);

  write_csv_row(csv, BenchmarkRow{image.width, image.height, custom, standard});

  std::cout << image.width << "x" << image.height << " -> "
            << "custom=" << custom.total_ms << " ms, "
            << "std=" << standard.total_ms << " ms, "
            << "reallocacoes_custom=" << custom.reallocations << ", "
            << "reallocacoes_std=" << standard.reallocations << '\n';
}

} // namespace benchmark