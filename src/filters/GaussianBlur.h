#pragma once

#include <omp.h>
#include <cmath>
#include <iostream>
#include <vector>
#include "../Image.h"

namespace imgr {
    class GaussianBlur {
    public:
        static std::vector<float> generate_gaussian_kernel(int kernel_size,
                                                           float sigma) {
            std::vector<float> kernel(kernel_size * kernel_size);
            float sum = 0.0;
            int radius = kernel_size / 2;

            for (int y = -radius; y <= radius; ++y) {
                for (int x = -radius; x <= radius; ++x) {
                    float exponent = -(x * x + y * y) / (2 * sigma * sigma);
                    float weight = std::exp(exponent);
                    int idx = (y + radius) * kernel_size + (x + radius);
                    kernel[idx] = weight;
                    sum += weight;
                }
            }

            // Normalize the kernel
            for (float& w : kernel) {
                w /= sum;
            }

            return kernel;
        }

        static void apply_gaussian_blur(Image& img, float sigma = 1.5f,
                                        int kernel_size = 5) {
            if (kernel_size % 2 == 0) {
                std::cout << "Kernel size must be odd number. Adjusting to "
                          << (kernel_size + 1) << std::endl;
                kernel_size += 1;
            }

            // Generate kernel
            auto kernel = generate_gaussian_kernel(kernel_size, sigma);

            // Copy of image
            Image original_img = img;
            int radius = kernel_size / 2;

            // Process each pixel
            for (int y = 0; y < img.m_height; ++y) {
                for (int x = 0; x < img.m_width; ++x) {
                    // Process each channel separately
                    for (int c = 0; c < img.m_channels; ++c) {
                        float pixel_value = 0.0f;
                        float kernel_sum = 0.0f;

                        // Apply kernel
                        for (int ky = -radius; ky <= radius; ++ky) {
                            for (int kx = -radius; kx <= radius; ++kx) {
                                int nx = std::max(
                                    0, std::min(x + kx, img.m_width - 1));
                                int ny = std::max(
                                    0, std::min(y + ky, img.m_height - 1));

                                // Get kernel weight
                                float weight =
                                    kernel[(ky + radius) * kernel_size +
                                           (kx + radius)];

                                // Get original pixel value
                                pixel_value +=
                                    original_img
                                        .m_data[(ny * img.m_width + nx) *
                                                    img.m_channels +
                                                c] *
                                    weight;
                                kernel_sum += weight;
                            }
                        }

                        // Normalize and update pixel
                        img.m_data[(y * img.m_width + x) * img.m_channels + c] =
                            static_cast<uint8_t>(std::min(
                                255.0f,
                                std::max(0.0f, pixel_value / kernel_sum)));
                    }
                }
            }
        }

        // Parallel Gaussian Blur with OpenMP
        static void apply_gaussian_blur_parallel(Image& img, float sigma = 1.5f,
                                                 int kernel_size = 5) {
            // Validate kernel size (must be odd)
            if (kernel_size % 2 == 0) {
                std::cerr << "Kernel size must be an odd number. Adjusting to "
                          << (kernel_size + 1) << std::endl;
                kernel_size += 1;
            }

            // Generate Gaussian kernel
            auto kernel = generate_gaussian_kernel(kernel_size, sigma);

            // Create a copy of the original image to read from
            Image original_img = img;
            int radius = kernel_size / 2;

            // Parallel processing of image
#pragma omp parallel for collapse(2)
            for (int y = 0; y < img.m_height; ++y) {
                for (int x = 0; x < img.m_width; ++x) {
                    // Process each channel separately
                    for (int c = 0; c < img.m_channels; ++c) {
                        float pixel_value = 0.0f;
                        float kernel_sum = 0.0f;

                        // Apply kernel
                        for (int ky = -radius; ky <= radius; ++ky) {
                            for (int kx = -radius; kx <= radius; ++kx) {
                                int nx = std::max(
                                    0, std::min(x + kx, img.m_width - 1));
                                int ny = std::max(
                                    0, std::min(y + ky, img.m_height - 1));

                                // Get kernel weight
                                float weight =
                                    kernel[(ky + radius) * kernel_size +
                                           (kx + radius)];

                                // Get original pixel value
                                pixel_value +=
                                    original_img
                                        .m_data[(ny * img.m_width + nx) *
                                                    img.m_channels +
                                                c] *
                                    weight;
                                kernel_sum += weight;
                            }
                        }

                        // Normalize and update pixel
                        img.m_data[(y * img.m_width + x) * img.m_channels + c] =
                            static_cast<uint8_t>(std::min(
                                255.0f,
                                std::max(0.0f, pixel_value / kernel_sum)));
                    }
                }
            }
        }
    };
}  // namespace imgr
