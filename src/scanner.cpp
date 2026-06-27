#include "../include/scanner.hpp"

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
