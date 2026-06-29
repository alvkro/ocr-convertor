#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "linkedqueue.hpp"
#include "vector.hpp"
#include <cstdint>

struct Pixel;

void process_image_streaming(const sc::vector<Pixel> &raw_image, int width,
                             LinkedQueue &queue);

uint8_t convert_to_gray(const Pixel &p);

#endif
