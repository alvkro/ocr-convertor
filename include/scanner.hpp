#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "./linkedqueue.hpp"
#include "./types.hpp"
#include "vector.hpp"
#include <cstdint>

void process_image_streaming(const sc::vector<Pixel> &raw_image, int width,
                             LinkedQueue &queue);

inline uint8_t convert_to_gray(const Pixel &p) {
  float x = 0.299f * p.r + 0.587f * p.g + 0.114f * p.b;

  return static_cast<uint8_t>(x);
}

#endif
