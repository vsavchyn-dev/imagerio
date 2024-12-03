#pragma once

#include <iostream>
#define STBI_MALLOC(sz) malloc(sz)
#define STBI_REALLOC(p, newsz) realloc(p, newsz)
#define STBI_FREE(p) free(p)

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#ifndef IMGR_IMAGE_H
#define IMGR_IMAGE_H

namespace imgr {

    struct Image {
        int m_width;
        int m_height;
        int m_channels;
        std::vector<uint8_t> m_data;
        std::string m_name;

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
                      << "\tWidth: " << m_width << "\n"
                      << "\tHeight:" << m_height << "\n"
                      << "\tChannels: " << m_channels << "\n"
                      << "\tSupposed data size: "
                      << m_width * m_height * m_channels << "\n"
                      << "\tSize of m_data: " << m_data.size() << "\n";
        }

        void load(const std::string& name) {
            if (name.empty()) {
                std::cerr << "Name is empty\n";
                return;
            }
            m_name = name;

            uint8_t* loaded_data = stbi_load(
                m_name.c_str(), &m_width, &m_height, &m_channels, m_channels);

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

        // TODO: finish write functionality

        // void write(std::string name = "") {
        //     if (name.empty())
        //         name = m_name;
        // }

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
