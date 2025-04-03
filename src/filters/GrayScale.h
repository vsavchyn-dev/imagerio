#pragma once

#ifndef IMGR_FILTER_GRAYSCALE_H
#  define IMGR_FILTER_GRAYSCALE_H

#  include <omp.h>

#  include <cmath>
#  include <iostream>
#  include <vector>

#  include "../Image.h"

namespace imgr {
class GrayScale {
 public:
  static void grayscaleImage(imgr::Image& img) {
    for (int y = 0; y < img.m_height; ++y) {
      for (int x = 0; x < img.m_width; ++x) {
        // Process each channel separately
        unsigned char gray = 0;
        for (int c = 0; c < img.m_channels; ++c) {
          gray = gray > img.m_data[(y * img.m_width + x) * img.m_channels]
                     ? gray
                     : img.m_data[(y * img.m_width + x) * img.m_channels];
        }
        for (int c = 0; c < 3; ++c) {
          img.m_data[(y * img.m_width + x) * img.m_channels + c] = gray;
        }
      }
    }
  }

  static void grayscaleImageParallel(imgr::Image& img) {
#  pragma omp parallel for collapse(2)
    for (int y = 0; y < img.m_height; ++y) {
      for (int x = 0; x < img.m_width; ++x) {
        // Process each channel separately
        unsigned char gray = 0;
        for (int c = 0; c < img.m_channels; ++c) {
          gray = gray > img.m_data[(y * img.m_width + x) * img.m_channels]
                     ? gray
                     : img.m_data[(y * img.m_width + x) * img.m_channels];
        }
        for (int c = 0; c < 3; ++c) {
          img.m_data[(y * img.m_width + x) * img.m_channels + c] = gray;
        }
      }
    }
  }
};
}  // namespace imgr

#endif  // !IMGR_FILTER_GRAYSCALE_H
