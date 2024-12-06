#pragma once

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
                          STBI_rgb_alpha);

            if (loaded_data == nullptr) {
                std::cerr << "Error by reading a file!\n";
                delete[] loaded_data;
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
                stbi_write_png(path.c_str(), m_width, m_height, m_channels,
                               m_data.data(), m_width * m_height);
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

        ~Image() {}
    };
}  // namespace imgr

#endif  // !IMGR_IMAGE_H
