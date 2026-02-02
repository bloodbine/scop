#include "includes/utils.hpp"

std::vector<std::string> split_string(const std::string src, char delim) {
    std::vector<std::string> split_strings;
    int last_start = 0;
    for (int i = 0; i <= (int)src.length(); i++) {
        if (src[i] == delim || src[i] == '\0') {
            split_strings.push_back(src.substr(last_start, i - last_start));
            last_start = i + 1;
        };
    };
    return (split_strings);
};
