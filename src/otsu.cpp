#include "../include/otsu.hpp"

void Otsu::fill_histogram() {
    for (size_t idx{0}; idx < size; ++idx) {
        int gray_shade = convert_to_gray(pixels[idx]);
        histogram[gray_shade]++;
    }
}

uint Otsu::calculate_otsu() {
    float total_sum{0};
    for (size_t idx{0}; idx < 256; ++idx) {
        total_sum += idx * histogram[idx];
    }

    float background_sum {0};
    int weight_background {0};
    int weight_foreground{0};

    float max_variancia{0};
    int ideal_threshold{0};

    for (size_t idx{0}; idx < 256; ++idx) {
        weight_background += histogram[idx]; // Pixels acumulados até t
        if (weight_background == 0) continue;

        weight_foreground = size - weight_background;
        if (weight_foreground == 0) break; // Acabaram os pixels da frente

        background_sum += (float)(idx * histogram[idx]);
    
        float media_background = background_sum / weight_background;
        float media_foreground = (total_sum - background_sum) / weight_foreground;

        float variancia = (float)weight_background * (float)weight_foreground * (media_background - media_foreground) * (media_background - media_foreground);

        if (variancia > max_variancia) {
            max_variancia = variancia;
            ideal_threshold = idx;
        }
    }

    return ideal_threshold;
}

void Otsu::binarize_img() {
    for (size_t idx{0}; idx < size; ++idx) {
        uint this_gray{ (convert_to_gray(pixels[idx]))};
        if (this_gray < this->calculate_otsu()) {
            pixels[idx].r = 0;
            pixels[idx].g = 0;
            pixels[idx].b = 0;
        } else {
            pixels[idx].r = 255;
            pixels[idx].g = 255;
            pixels[idx].b = 255;
        }
    }
}