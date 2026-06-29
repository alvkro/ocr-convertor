#include "../include/scanner.hpp"
#include "../include/types.hpp"

void process_image_streaming(const sc::vector<Pixel> &raw_image, int width,
                             LinkedQueue &queue) {
  sc::vector<uint8_t> current_line{};

  for (size_t idx{ 0 }; idx < raw_image.size(); idx++) {
    current_line.push_back(convert_to_gray(raw_image[idx]));

    if (current_line.size() == static_cast<size_t>(width)) {
      queue.enqueue(std::move(current_line));

      current_line = sc::vector<uint8_t>();
    }
  }
}

uint8_t convert_to_gray(const Pixel &p) {
  float x = 0.299f * p.r + 0.587f * p.g + 0.114f * p.b;

  return static_cast<uint8_t>(x);
}
