#include "../include/otsu.hpp"

void Otsu::fill_histogram() {
    for (size_t idx{0}; idx < size; ++idx) {
        int gray_shade = convert_to_gray(m_pixels[idx]);
        histogram[gray_shade]++;
    }
}

uint8_t Otsu::calculate_otsu() {
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

void Otsu::binarize_img(uint8_t threshold) {
    for (size_t idx{0}; idx < size; ++idx) {
        uint this_gray{ (convert_to_gray(m_pixels[idx]))};
        if (this_gray < threshold) {
            m_pixels[idx].r = 0;
            m_pixels[idx].g = 0;
            m_pixels[idx].b = 0;
        } else {
            m_pixels[idx].r = 255;
            m_pixels[idx].g = 255;
            m_pixels[idx].b = 255;
        }
    }
}

uint8_t Otsu::convert_to_gray(Pixel& p) {
  float x = 0.299f * p.r + 0.587f * p.g + 0.114f * p.b;

  return static_cast<uint8_t>(x);
}