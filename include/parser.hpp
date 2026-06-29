#pragma once
#include "vector.hpp"
#include <string>

struct Pixel;

struct ImagemInfo {
    int w;
    int h;
    bool is_color; // true = PPM, false = PGM
};

ImagemInfo read_image(const std::string& path, sc::vector<Pixel>& pixels);
void save_bin(const std::string& path, sc::vector<Pixel>& pixels, int w, int h);