//
// Created by Administrator on 2022/10/9.
//

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include "fast_gaussian_blur.h"

#include <linux/can.h>
#include <linux/can/raw.h>

auto const epoch = std::chrono::steady_clock::now();
double now() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - epoch).count() / 1000.0;
};


using namespace std;

int main() {
    std::cout << "Hello, STB_Image" << std::endl;

    char   buffer[1000];
    string path = getcwd(buffer, 1000);

    string inputPath = path + R"(C:\Users\Administrator\CLionProjects\C17\stb\girl.jpg)";
    int iw, ih, n;

    // 加载图片获取宽、高、颜色通道信息
    unsigned char *idata = stbi_load(R"(C:\Users\Administrator\CLionProjects\C17\stb\girl.jpg)", &iw, &ih, &n, 0);

//    int ow = iw / 2;
//    int oh = ih / 2;
//    auto *odata = (unsigned char *) malloc(ow * oh * n);

//    // 改变图片尺寸
//    stbir_resize(idata, iw, ih, 0, odata, ow, oh, 0, STBIR_TYPE_UINT8, n, STBIR_ALPHA_CHANNEL_NONE, 0,
//                 STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP,
//                 STBIR_FILTER_BOX, STBIR_FILTER_BOX,
//                 STBIR_COLORSPACE_SRGB, nullptr
//    );

    std::size_t size = iw * ih * n;
    auto * new_image = new unsigned char[size];
    auto * old_image = new unsigned char[size];

    // channels copy r,g,b
    for(std::size_t i = 0; i < size; ++i)
    {
        old_image[i] = idata[i];
    }


    fast_gaussian_blur(old_image, new_image, iw, ih, n, 10, 3);


    // convert result
    for(std::size_t i = 0; i < size; ++i)
    {
        idata[i] = (unsigned char )(new_image[i]);
    }

    string outputPath = R"(C:\Users\Administrator\CLionProjects\C17\stb\girl_out.jpg)";
//     写入图片
//    stbi_write_png(outputPath.c_str(), ow, oh, n, odata, 0);

//    stbi_write_png(outputPath.c_str(), iw, ih, n, new_image, 0);

    stbi_write_jpg(outputPath.c_str(),iw, ih, n, idata, 90);

    stbi_image_free(idata);
//    stbi_image_free(odata);
    return 0;
}