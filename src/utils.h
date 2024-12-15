#pragma once

#if __cplusplus < 202002L  // pre C++20
#include <fstream>
#include <string>

static bool ends_with(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

static bool starts_with(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() &&
           str.compare(0, prefix.size(), prefix) == 0;
}

// some extra helper overloads:

static bool ends_with(const std::string& str, const char* suffix,
                      unsigned suffixLen) {
    return str.size() >= suffixLen &&
           str.compare(str.size() - suffixLen, suffixLen, suffix, suffixLen) ==
               0;
}

static bool ends_with(const std::string& str, const char* suffix) {
    return ends_with(str, suffix, std::string::traits_type::length(suffix));
}

static bool starts_with(const std::string& str, const char* prefix,
                        unsigned prefixLen) {
    return str.size() >= prefixLen &&
           str.compare(0, prefixLen, prefix, prefixLen) == 0;
}

static bool starts_with(const std::string& str, const char* prefix) {
    return starts_with(str, prefix, std::string::traits_type::length(prefix));
}

static bool is_valid_path(const std::string& path) {
    if (path.empty())
        return false;

    try {
        std::filesystem::path fs_path(path);

        // Check if path is absolute or relative
        if (!fs_path.is_absolute() && !fs_path.is_relative())
            return false;

        return true;
    } catch (...) {
        return false;
    }
}

static bool is_valid_extension(
    const std::string& path, const std::vector<std::string>& valid_extensions) {
    if (path.empty() || valid_extensions.empty())
        return false;

    std::string local_path(path.length(), '\0');

    for (size_t i = 0; i < path.length(); i++) {
        local_path[i] = std::tolower(path[i]);
    }

    for (size_t i = 0; i < valid_extensions.size(); i++) {
        if (ends_with(path, valid_extensions[i])) {
            return true;
        }
    }

    return false;
}
#endif
