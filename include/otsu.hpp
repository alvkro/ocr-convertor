#include "parser.hpp"
#include "types.hpp"

class Otsu {
private:
    int width{0};
    int height{0};
    int size{0};
    int histogram[256] = { 0 };
    sc::vector<Pixel> m_pixels;
    uint8_t convert_to_gray(Pixel &p);
public:
    Otsu(const ImagemInfo& img_info, const sc::vector<Pixel>& img_pixels) : m_pixels(img_pixels)    {
        width = img_info.w;
        height = img_info.h;
        size = img_info.w * img_info.h;
    }

    uint calculate_otsu();
    void fill_histogram();
    void binarize_img();
};