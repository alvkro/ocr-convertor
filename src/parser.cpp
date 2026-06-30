#include "../include/parser.hpp"
#include "../include/types.hpp"
#include <cstdint>
#include <fstream>
#include <stdexcept>

ImagemInfo read_image(const std::string& path, sc::vector<Pixel>& pixels) {
    std::ifstream arq(path);
    int w, h, maxVal;
    if (!arq.is_open()) {
        throw std::runtime_error("Erro ao abrir: " + path);
    }
    std::string type;
    arq >> type;

    if (type != "P3" && type != "P2") {
        throw std::runtime_error("Formato inválido: " + type);
    }
    arq >> w >> h >> maxVal;

    for (int i = 0; i < w * h; i++) {
        Pixel temp_p;
        int r {0}, g {0}, b {0};
        if (type == "P3") {
            arq >> r >> g >> b;
        } else {
            arq >> r;
            r = g = b;
        }
        temp_p.r = (uint8_t)r;
        temp_p.g = (uint8_t)g;
        temp_p.b = (uint8_t)b;
        pixels.push_back(temp_p);
    }
    return {w, h, true};
}

void save_bin(const std::string& path, sc::vector<Pixel>& pixels, int w, int h) {
    std::ofstream arquivo(path, std::ios::binary);
    if (!arquivo.is_open())
        throw std::runtime_error("Erro ao salvar: " + path);

    arquivo.write((char*)&w, sizeof(int));
    arquivo.write((char*)&h, sizeof(int));

    int total_bytes = pixels.size() * sizeof(Pixel);
    arquivo.write((char*)pixels.data(), total_bytes);
}