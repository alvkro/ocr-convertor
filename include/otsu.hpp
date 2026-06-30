#include "parser.hpp"
#include "types.hpp"

class Otsu {
private:
    int width{0};
    int height{0};
    int size{0};
    int histogram[256] = { 0 };
    sc::vector<Pixel> pixels;
    uint convert_to_gray(Pixel &p);
public:
    Otsu(ImagemInfo &img_info, sc::vector<Pixel> &img_pixels) {
        width = img_info.w;
        height = img_info.h;
        size = img_info.w * img_info.h;
        pixels = img_pixels;
    }

    uint calculate_otsu();
    void fill_histogram();
    void binarize_img();
};