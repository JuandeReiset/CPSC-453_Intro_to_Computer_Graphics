/**
* @author	Andrew Robert Owens, Philmo Gu
* @date 	2019-10-15.
* @details Organization: Biological Modeling and Visualization research group
* University of Calgary, Calgary, AB, Canada
*
* Contact:	arowens [at] ucalgary.ca
* @copyright Copyright (c) 2019 ___ALGORITHMIC_BOTANY___. All rights reserved.
*
* @brief
*/

#pragma once

#include <iosfwd>
#include <vector>
#include <array>
#include <memory>
#include <cstddef>
#include "grid2.hpp"
#include "vec3f.hpp"

namespace raster {

struct RGB {
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

inline RGB convertToRGB(math::Vec3f const &color) {
  auto clamp = [](float t) {
    if (t < 0.f)
      t = 0.f;
    if (t > 1.f)
      t = 1.f;
    return t;
  };
  RGB rgb;
  rgb.r = clamp(color.x) * 255;
  rgb.g = clamp(color.y) * 255;
  rgb.b = clamp(color.z) * 255;
  return rgb;
}

class Image {
public:
  using data_ptr = std::unique_ptr<unsigned char, void (*)(void *)>;

public:
  Image(data_ptr data, uint32_t width, uint32_t height, uint8_t channels);

  unsigned char const *data() const;
  unsigned char *data();

  uint32_t width() const;
  uint32_t height() const;
  uint8_t channels() const;
  bool isEmpty() const;

private:
  data_ptr m_data;
  uint32_t m_width;
  uint32_t m_height;
  uint8_t m_channels;
};

Image read_image_from_file(char const *filename);

Image read_image_from_file_and_flipVertically(char const *filename);

int write_image_to_png(char const *filename, Image const &image);

int write_screen_to_file(char const *filename,
                         geometry::Grid2<RGB> const &sceen);

} // namespace
