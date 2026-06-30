#pragma once

#include "../include/types.hpp"
#include "../include/vector.hpp"

#include <array>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>

namespace benchmark
{

    struct ImageData
    {
        int width{0};
        int height{0};
        std::vector<Pixel> pixels;
    };

    struct PhaseTimes
    {
        double build_ms{0.0};
        double histogram_ms{0.0};
        double otsu_ms{0.0};
        double binarization_ms{0.0};
        double total_ms{0.0};
        std::size_t reallocations{0};
        std::uint8_t threshold{0};
        std::uint64_t checksum{0};
    };

    struct BenchmarkRow // classe da linha do benchmark
    {
        int width{0};
        int height{0};
        PhaseTimes custom;
        PhaseTimes standard;
    };

    Pixel make_pixel(int x, int y);
    std::vector<Pixel> generate_source_image(int width, int height);
    ImageData load_image_from_file(const std::string &path);
    uint8_t convert_to_gray(const Pixel &p);

    template <typename Container>
    std::size_t build_pixels(const std::vector<Pixel> &source, Container &pixels)
    {
        std::size_t reallocations{0};
        std::size_t previous_capacity{pixels.capacity()};

        for (const Pixel &pixel : source)
        {
            pixels.push_back(pixel);

            const std::size_t current_capacity{pixels.capacity()};
            if (current_capacity != previous_capacity)
            {
                ++reallocations;
                previous_capacity = current_capacity;
            }
        }

        if constexpr (std::is_same_v<Container, sc::vector<Pixel>>)
        {
            return pixels.get_resize_count(); // getter interno de sc::vector
        }

        return reallocations;
    }

    template <typename Container>
    std::array<std::uint32_t, 256> build_histogram(const Container &pixels)
    {
        std::array<std::uint32_t, 256> histogram{};

        for (std::size_t idx = 0; idx < pixels.size(); ++idx)
        {
            ++histogram[convert_to_gray(pixels[idx])];
        }

        return histogram;
    }

    uint8_t calculate_otsu_threshold(const std::array<std::uint32_t, 256> &histogram,
                                     std::size_t total_pixels);

    template <typename Container>
    std::uint64_t binarize_image(Container &pixels, uint8_t threshold)
    {
        std::uint64_t checksum{0};

        for (std::size_t idx = 0; idx < pixels.size(); ++idx)
        {
            const uint8_t gray = convert_to_gray(pixels[idx]);
            const uint8_t value = gray < threshold ? 0 : 255;

            pixels[idx].r = value;
            pixels[idx].g = value;
            pixels[idx].b = value;

            checksum += static_cast<std::uint64_t>(pixels[idx].r);
            checksum += static_cast<std::uint64_t>(pixels[idx].g);
            checksum += static_cast<std::uint64_t>(pixels[idx].b);
        }

        return checksum;
    }

    template <typename Container>
    PhaseTimes run_pipeline_for_container(const std::vector<Pixel> &source)
    {
        PhaseTimes result{};
        Container pixels;

        /*std::chrono::steady_clock::now() retorna o momento exato em que 
        a função é executada, capturando um "ponto no tempo"*/
        const auto total_start = std::chrono::steady_clock::now();

        const auto build_start = std::chrono::steady_clock::now();
        result.reallocations = build_pixels(source, pixels);
        const auto build_end = std::chrono::steady_clock::now();

        const auto histogram_start = std::chrono::steady_clock::now();
        const auto histogram = build_histogram(pixels);
        const auto histogram_end = std::chrono::steady_clock::now();

        const auto otsu_start = std::chrono::steady_clock::now();
        result.threshold = calculate_otsu_threshold(histogram, pixels.size());
        const auto otsu_end = std::chrono::steady_clock::now();

        const auto binarization_start = std::chrono::steady_clock::now();
        result.checksum = binarize_image(pixels, result.threshold);
        const auto binarization_end = std::chrono::steady_clock::now();

        const auto total_end = std::chrono::steady_clock::now();

        result.build_ms = std::chrono::duration<double, std::milli>(build_end - build_start).count();
        result.histogram_ms = std::chrono::duration<double, std::milli>(histogram_end - histogram_start).count();
        result.otsu_ms = std::chrono::duration<double, std::milli>(otsu_end - otsu_start).count();
        result.binarization_ms = std::chrono::duration<double, std::milli>(binarization_end - binarization_start).count();
        result.total_ms = std::chrono::duration<double, std::milli>(total_end - total_start).count();

        return result;
    }

    void write_csv_header(std::ofstream &csv);
    void write_csv_row(std::ofstream &csv, const BenchmarkRow &row);
    bool parse_arguments(int argc, char **argv, std::string &output_path, std::string &image_path, bool &use_input_image);
    void run_synthetic_benchmark(std::ofstream &csv);
    void run_image_benchmark(std::ofstream &csv, const std::string &image_path);

} // namespace benchmark