#if __cplusplus < 202002L  // pre C++20
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
#endif
