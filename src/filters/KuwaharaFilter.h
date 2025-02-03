#pragma once

#ifndef IMGR_FILTER_KUWAHARA_H
#define IMGR_FILTER_KUWAHARA_H

#include <omp.h>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
#include "../Image.h"
#include "GaussianBlur.h"

namespace imgr {
    class KuwaharaFilter {
    public:
        static void apply_kuwara_filter(imgr::Image& image,
                                        int window_size = 7) {
            if (window_size < 5 || window_size % 2 == 0) {
                std::cerr << "Invalid winsize " << window_size
                          << ": winsize must follow formula: w = 4*n+1.\n";
                return;
            }

            if (image.m_channels < 1) {
                std::cerr << "Image has no channels to process.\n";
                return;
            }

            // imgr::Image::rgb_to_hsv(image);
            imgr::GaussianBlur::apply_gaussian_blur_parallel(image, 2.0, 11);

            Image filtered_image = image;

            const size_t window_size_half = window_size / 2;
            const size_t num_regions =
                (window_size == 5) ? 4 : ((window_size == 7) ? 9 : 16);
            const size_t num_regions_sqrt = std::sqrt(num_regions);
            const size_t region_size = window_size / num_regions_sqrt;

            for (size_t y = window_size_half;
                 y < image.m_height - window_size_half; y++) {
                for (size_t x = window_size_half;
                     x < image.m_width - window_size_half; x++) {
                    std::vector<std::vector<double>> means(
                        num_regions,
                        std::vector<double>(image.m_channels, 0.0));
                    std::vector<std::vector<double>> variances(
                        num_regions,
                        std::vector<double>(image.m_channels, 0.0));

                    size_t region_idx = 0;

                    for (size_t dy = 0; dy < num_regions_sqrt; dy++) {
                        for (size_t dx = 0; dx < num_regions_sqrt; dx++) {
                            size_t start_region_x =
                                x - window_size_half + dx * region_size;
                            size_t start_region_y =
                                y - window_size_half + dy * region_size;

                            std::vector<double> sums(image.m_channels, 0.0);
                            std::vector<double> sums_squared(image.m_channels,
                                                             0.0);

                            for (size_t py = start_region_y;
                                 py < start_region_y + region_size; py++) {
                                for (size_t px = start_region_x;
                                     px < start_region_x + region_size; px++) {
                                    const size_t idx =
                                        (py * image.m_width + px) *
                                        image.m_channels;

                                    for (int channel = 0;
                                         channel < image.m_channels;
                                         channel++) {
                                        const double pixel_value =
                                            image.m_data[idx + channel];
                                        sums[channel] += pixel_value;
                                        sums_squared[channel] +=
                                            pixel_value * pixel_value;
                                    }
                                }
                            }

                            double area = region_size * region_size;
                            for (int channel = 0; channel < image.m_channels;
                                 channel++) {
                                means[region_idx][channel] =
                                    sums[channel] / area;
                                variances[region_idx][channel] =
                                    (sums_squared[channel] / area) -
                                    (means[region_idx][channel] *
                                     means[region_idx][channel]);
                            }
                            region_idx++;
                        }
                    }

                    size_t best_region = 0;
                    double min_vatriance = std::numeric_limits<double>::max();
                    for (size_t i = 0; i < num_regions; i++) {
                        double total_variance = 0.0;
                        for (int channel = 0; channel < image.m_channels;
                             channel++) {
                            total_variance += variances[i][channel];
                        }
                        if (total_variance < min_vatriance) {
                            min_vatriance = total_variance;
                            best_region = i;
                        }
                    }

                    size_t filtered_idx =
                        (y * image.m_width + x) * image.m_channels;

                    for (int channel = 0; channel < image.m_channels;
                         channel++) {
                        filtered_image.m_data[filtered_idx + channel] =
                            static_cast<uint8_t>(means[best_region][channel]);
                    }
                }
            }

            image = filtered_image;

            // imgr::Image::hsv_to_rgb(image);
        }
    };
}  // namespace imgr

#endif  // !IMGR_FILTER_KUWAHARA_H
