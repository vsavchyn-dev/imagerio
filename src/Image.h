#pragma once

#include <cmath>
#define STBI_MALLOC(sz) malloc(sz)
#define STBI_REALLOC(p, newsz) realloc(p, newsz)
#define STBI_FREE(p) free(p)

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include "utils.h"

#ifndef IMGR_IMAGE_H
#define IMGR_IMAGE_H

namespace imgr {

    struct Image {
        int m_width;
        int m_height;
        int m_channels;
        std::vector<uint8_t> m_data;
        std::string m_name;
        std::string m_file_path;

        Image()
            : m_width(0), m_height(0), m_channels(0), m_data(), m_name("") {}

        Image(const std::string& name)
            : m_width(0), m_height(0), m_channels(0), m_data(), m_name("") {
            load(name);
        }

        void clear() {
            m_width = 0;
            m_height = 0;
            m_channels = 0;
            m_data = {};
            m_name = "";
        }

        void print_stats() const {
            if (m_name.empty()) {
                std::cout << "Image: Is empty\n";
                return;
            }

            std::cout << "Image: " << m_name << "\n"
                      << "\tPath: " << m_file_path << "\n"
                      << "\tWidth: " << m_width << "\n"
                      << "\tHeight:" << m_height << "\n"
                      << "\tChannels: " << m_channels << "\n"
                      << "\tSupposed data size: "
                      << m_width * m_height * m_channels << "\n"
                      << "\tSize of m_data: " << m_data.size() << "\n";
        }

        void load(const std::string& path = "") {
            if (path.empty()) {
                std::cerr << "Name is empty\n";
                return;
            }

            // TODO: check name ext - throw error if wrong.

            m_file_path = path;

            size_t pos_for_name = path.find_last_of('/');
            if (pos_for_name != std::string::npos) {
                m_name = path.substr(pos_for_name + 1,
                                     path.size() - pos_for_name + 1);
            } else {
                m_name = path;
            }

            uint8_t* loaded_data =
                stbi_load(m_file_path.c_str(), &m_width, &m_height, &m_channels,
                          STBI_default);

            if (loaded_data == nullptr) {
                std::cerr << "Error by reading a file!\n";
                return;
            }

            size_t full_size = m_width * m_height * m_channels;
            m_data.clear();
            m_data.reserve(full_size + 1);
            copy(&loaded_data[0], &loaded_data[full_size],
                 back_inserter(m_data));
            delete[] loaded_data;
        }

        void write(std::string path = "") {
            if (path.empty() && m_name.empty()) {
                std::cerr << "Passed argument and image struct name are empty "
                             "- nothing to write\n";
                return;
            }

            if (path.empty()) {
                std::cout << "Passed name is empty, creating a new one with "
                             "\"copy\"\n";

                size_t dot_idx = m_name.find_last_of('.');

                std::string name_str = m_name.substr(0, dot_idx);
                std::string extension_str =
                    m_name.substr(dot_idx, m_name.length() - dot_idx + 1);
                std::string copy_str = "-copy";

                path = name_str + copy_str + extension_str;

                std::cout << "\tDot index: " << dot_idx << "\n"
                          << "\tName: " << name_str << "\n"
                          << "\tExtension: " << extension_str << "\n"
                          << "\tNew name var: " << path << "\n";
            }

            // TODO: check ext to write -> throw error if smth wrong/show that we had problems
            // return;

            if (ends_with(path, ".png")) {
                int stride = m_width * m_channels;
                stbi_write_png(path.c_str(), m_width, m_height, m_channels,
                               m_data.data(), stride);
            } else if (ends_with(path, ".jpg") || ends_with(path, ".jpeg")) {
                stbi_write_jpg(path.c_str(), m_width, m_height, m_channels,
                               m_data.data(), 100);
            } else {
                std::cerr << "Unsupported file type to write\n";
                return;
            }

            std::cout << "Wrote a image called \"" << m_name
                      << "\" into the file: " << path << "\n";
        }

        Image& operator=(const Image& other_img) {
            m_width = other_img.m_width;
            m_height = other_img.m_height;
            m_channels = other_img.m_channels;
            m_data = other_img.m_data;
            m_name = other_img.m_name;

            return *this;
        }

        static void rgb_to_hsv(Image& image) {
            if (image.m_data.empty()) {
                std::cerr << "Empty image to convert to HSV!\n";
                return;
            }

            for (size_t i = 0; i < image.m_data.size(); i += 3) {
                const double r_prime =
                    static_cast<double>(image.m_data[i]) / 255.0f;
                const double g_prime =
                    static_cast<double>(image.m_data[i + 1]) / 255.0;
                const double b_prime =
                    static_cast<double>(image.m_data[i + 2]) / 255.0;
                const double c_max = std::max({r_prime, g_prime, b_prime});
                const double c_min = std::min({r_prime, g_prime, b_prime});
                const double diff = c_max - c_min;

                double hue = 0.0;
                double sat = (c_max == 0.0) ? 0.0 : (diff / c_max) * 100.0;
                double val = c_max * 100.0;

                if (c_max == r_prime) {
                    hue = 60.0 * std::fmod((g_prime - b_prime) / diff, 6.0);
                } else if (c_max == g_prime) {
                    hue = 60.0 * (b_prime - r_prime) / diff + 2.0;
                } else if (c_max == b_prime) {
                    hue = 60.0 * (r_prime - g_prime) / diff + 4.0;
                }

                if (hue < 0)
                    hue += 360.0;

                image.m_data[i] = static_cast<uint8_t>(hue);
                image.m_data[i + 1] = static_cast<uint8_t>(sat);
                image.m_data[i + 2] = static_cast<uint8_t>(val);
            }
        }

        static void hsv_to_rgb(Image& image) {
            if (image.m_data.empty()) {
                std::cerr << "Empty image to convert to RBG!\n";
                return;
            }

            for (size_t i = 0; i < image.m_data.size(); i += 3) {
                const double hue = static_cast<double>(image.m_data[i]);
                const double sat = static_cast<double>(image.m_data[i + 1]);
                const double val = static_cast<double>(image.m_data[i + 2]);

                const double c = val * sat;
                const double x =
                    c * (1 - std::abs(std::fmod(hue / 60.0, 2.0) - 1));
                const double m = val - c;

                double r_prime = 0.0;
                double g_prime = 0.0;
                double b_prime = 0.0;

                if (hue >= 0.0 && hue < 60.0) {
                    r_prime = c;
                    g_prime = x;
                } else if (hue >= 60.0 && hue < 120.0) {
                    r_prime = x;
                    g_prime = c;
                } else if (hue >= 120.0 && hue < 180.0) {
                    g_prime = c;
                    b_prime = x;
                } else if (hue >= 180.0 && hue < 240.0) {
                    g_prime = x;
                    b_prime = c;
                } else if (hue >= 240.0 && hue < 300.0) {
                    r_prime = x;
                    b_prime = c;
                } else if (hue >= 300.0 && hue < 360.0) {
                    r_prime = c;
                    b_prime = x;
                }

                const uint8_t r = static_cast<uint8_t>((r_prime + m) * 255.0);
                const uint8_t g = static_cast<uint8_t>((g_prime + m) * 255.0);
                const uint8_t b = static_cast<uint8_t>((b_prime + m) * 255.0);

                image.m_data[i] = r;
                image.m_data[i + 1] = g;
                image.m_data[i + 2] = b;
            }
        }

        ~Image() {}
    };
}  // namespace imgr

#endif  // !IMGR_IMAGE_H
